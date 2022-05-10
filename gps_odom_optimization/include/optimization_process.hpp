#include "ceres_structs.hpp"

class OptimizationProcess;
typedef OptimizationProcess* OptimizationProcessPtr;

class OptimizationProcess {
public:
	OptimizationProcess(void);
	~OptimizationProcess() { }

	void addOdometryConstraint (OdometryConstraint constraint_odom){
		constraints_odom_.push_back(constraint_odom);
		if (constraints_odom_.size() > window_size_){
			constraints_odom_.erase(constraints_odom_.begin());
		}
	}
	void addPriorConstraint (PriorConstraint constraint_prior){
		constraints_prior_.push_back(constraint_prior);
		if (constraints_prior_.size() > window_size_){
			constraints_prior_.erase(constraints_prior_.begin());
		}
	}
	void addPose3dToTrajectoryEstimated (Pose3dWithCovariance pose3d_estimated){
		trajectory_estimated_.push_back(pose3d_estimated);
		if (trajectory_estimated_.size() > window_size_){
			trajectory_estimated_.erase(trajectory_estimated_.begin());
		}
	}
	void addPose3dToTrajectoryOdom (Pose3dWithCovariance pose3d_odom){
		trajectory_odom_.push_back(pose3d_odom);
		if (trajectory_odom_.size() > window_size_){
			trajectory_odom_.erase(trajectory_odom_.begin());
		}
	}
	Trajectory getTrajectoryEstimated (void){
		return trajectory_estimated_;
	}
	Trajectory getTrajectoryOdom (void){
		return trajectory_odom_;
	}

	void generateOdomResiduals (ceres::LossFunction* loss_function,
			                    ceres::LocalParameterization* quaternion_local_parameterization,
								ceres::Problem* problem);
	void generatePriorResiduals (ceres::LossFunction* loss_function,
			                     ceres::LocalParameterization* quaternion_local_parameterization,
								 ceres::Problem* problem);
	void solveOptimizationProblem (ceres::Problem* problem);
	void estimateCovariance (ceres::Problem* problem);
	void propagateState (size_t index);

	Pose3dWithCovariance parsePose2dToPose3d (size_t id, double x, double y, double w, Eigen::Matrix<double, 6, 6> cov);

private:
	OdometryConstraintsVector constraints_odom_;
	PriorConstraintVector constraints_prior_;
	Trajectory trajectory_odom_;
	Trajectory trajectory_estimated_;

	int window_size_;
};

OptimizationProcess::OptimizationProcess(void) {
	window_size_ = 50;
	return;
}

Pose3dWithCovariance parsePose2dToPose3d (size_t id, double x, double y, double w, Eigen::Matrix<double, 6, 6> cov)
{
	Pose3dWithCovariance pose3d;

	pose3d.id = id;
	pose3d.p.x() = x;
	pose3d.p.y() = y;
	pose3d.p.z() = 0.0;

	Eigen::AngleAxisd yaw_angle(w, Eigen::Vector3d::UnitZ());
	pose3d.q = yaw_angle;

	pose3d.covariance = cov;

	return pose3d;
}

void OptimizationProcess::propagateState (size_t index)
{
	if (index == 0){
		addPose3dToTrajectoryEstimated(trajectory_odom_.at(index));
	}else{
		//// Generate transform
        Eigen::Matrix<double, 3, 1> p_a(trajectory_odom_.at(index-1).p);
        Eigen::Quaternion<double> q_a(trajectory_odom_.at(index-1).q);
        Eigen::Matrix<double, 3, 1> p_b(trajectory_odom_.at(index).p);
        Eigen::Quaternion<double> q_b(trajectory_odom_.at(index).q);

        // Compute the relative transformation between the two frames.
        Eigen::Quaternion<double> q_a_inverse = q_a.conjugate();
        Eigen::Quaternion<double> q_ab_estimated = q_a_inverse * q_b;

        // Represent the displacement between the two frames in the A frame.
        Eigen::Matrix<double, 3, 1> p_ab_estimated = p_b - p_a;
        ////

        ////Propagate pose
        Pose3dWithCovariance propagated_pose;
        propagated_pose.id = trajectory_odom_.at(index).id;
        propagated_pose.p = trajectory_estimated_.at(trajectory_estimated_.size()-1).p + p_ab_estimated;
        propagated_pose.q = trajectory_estimated_.at(trajectory_estimated_.size()-1).q * q_ab_estimated;
        propagated_pose.covariance = trajectory_estimated_.at(trajectory_estimated_.size()-1).covariance;

        addPose3dToTrajectoryEstimated(propagated_pose);
	}

	return;
}

void OptimizationProcess::generateOdomResiduals(ceres::LossFunction* loss_function,
		                                        ceres::LocalParameterization* quaternion_local_parameterization,
												ceres::Problem* problem)
{
	//// Generate residuals
	for (int j = 0; j < constraints_odom_.size(); j++){

		size_t index_b;
		size_t index_e;
		bool exist_b = false;
		bool exist_e = false;

		//std::cout << "j: " << j << std::endl;

		for (int i = 0; i < trajectory_estimated_.size(); i++){
			if (trajectory_estimated_.at(i).id == constraints_odom_.at(j).id_begin){
				index_b = i;
				exist_b = true;
			}
			if (trajectory_estimated_.at(i).id == constraints_odom_.at(j).id_end){
				index_e = i;
				exist_e = true;
			}
		}

		//std::cout << "index_b: " << index_b << ", exist_e: " << exist_e << std::endl;

        if (exist_b && exist_e){
			ceres::CostFunction* cost_function_odom = OdometryErrorTerm::Create(constraints_odom_.at(j).tf_p,
																				constraints_odom_.at(j).tf_q,
																				constraints_odom_.at(j).information);
			problem->AddResidualBlock(cost_function_odom,
									  loss_function,
									  trajectory_estimated_.at(index_b).p.data(),
									  trajectory_estimated_.at(index_b).q.coeffs().data(),
									  trajectory_estimated_.at(index_e).p.data(),
									  trajectory_estimated_.at(index_e).q.coeffs().data());

			problem->SetParameterization(trajectory_estimated_.at(index_b).q.coeffs().data(), quaternion_local_parameterization);
			problem->SetParameterization(trajectory_estimated_.at(index_e).q.coeffs().data(), quaternion_local_parameterization);
        }
	}
	return;
}

void OptimizationProcess::generatePriorResiduals(ceres::LossFunction* loss_function,
		                                         ceres::LocalParameterization* quaternion_local_parameterization,
												 ceres::Problem* problem)
{
	//// Generate residuals
	for (int j = 0; j < constraints_prior_.size(); j++){

		size_t index;
		bool exist = false;

		for (int i = 0; i < trajectory_estimated_.size(); i++){
			if (trajectory_estimated_.at(i).id == constraints_prior_.at(j).id){
				index = i;
				exist = true;
			}
		}

		if (exist){
			ceres::CostFunction* cost_function_prior = PriorErrorTerm::Create(constraints_prior_.at(j).p,
																			  constraints_prior_.at(j).q,
																			  constraints_prior_.at(j).information);
			problem->AddResidualBlock(cost_function_prior,
									  loss_function,
									  trajectory_estimated_.at(index).p.data(),
									  trajectory_estimated_.at(index).q.coeffs().data());
			problem->SetParameterization(trajectory_estimated_.at(index).q.coeffs().data(), quaternion_local_parameterization);
		}
	}
	return;
}

void OptimizationProcess::solveOptimizationProblem(ceres::Problem* problem)
{
    //CHECK(problem != NULL);
    ceres::Solver::Options options;
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::SPARSE_NORMAL_CHOLESKY;
    ceres::Solver::Summary summary;
    std::cout << "Pre-solve" << std::endl;
    ceres::Solve(options, problem, &summary);
    std::cout << "Post-solve" << std::endl;
    //std::cout << summary.FullReport() << '\n';
	return;
}


/*



void OptimizationProcess::estimateCovariance(ceres::Problem* problem)
{
	ceres::Covariance::Options options;
	ceres::Covariance covariance(options);

	size_t index = trajectory_estimated_.size() -1;

	std::vector<std::pair<const double*, const double*> > covariance_blocks;
	covariance_blocks.push_back(std::make_pair(trajectory_estimated_.at(index).p.data(), trajectory_estimated_.at(index).p.data()));
	covariance_blocks.push_back(std::make_pair(trajectory_estimated_.at(index).q.coeffs().data(), trajectory_estimated_.at(index).q.coeffs().data()));

	CHECK(covariance.Compute(covariance_blocks, problem));

	double covariance_pp[3 * 3];
	double covariance_qq[4 * 4];
	covariance.GetCovarianceBlock(trajectory_estimated_.at(index).p.data(), trajectory_estimated_.at(index).p.data(), covariance_pp);
	covariance.GetCovarianceBlock(trajectory_estimated_.at(index).q.coeffs().data(), trajectory_estimated_.at(index).q.coeffs().data(), covariance_qq);

	trajectory_estimated_.at(index).covariance(0, 0) = covariance_pp[0];
	trajectory_estimated_.at(index).covariance(0, 1) = covariance_pp[1];
	trajectory_estimated_.at(index).covariance(1, 0) = covariance_pp[3];
	trajectory_estimated_.at(index).covariance(1, 1) = covariance_pp[4];

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (trajectory_estimated_.at(index).q.w() * trajectory_estimated_.at(index).q.z() + trajectory_estimated_.at(index).q.x() * trajectory_estimated_.at(index).q.y());
    double cosy_cosp = 1 - 2 * (trajectory_estimated_.at(index).q.y() * trajectory_estimated_.at(index).q.y() + trajectory_estimated_.at(index).q.z() * trajectory_estimated_.at(index).q.z());
    double yaw = std::atan2(siny_cosp, cosy_cosp);

    double x = covariance_qq[0];
    double y = covariance_qq[5];
    double z = covariance_qq[10];
    double w = covariance_qq[15];

    // yaw (z-axis rotation)
    siny_cosp = 2 * (w * z + x * y);
    cosy_cosp = 1 - 2 * (y * y + z * z);
    double yaw_cov = std::atan2(siny_cosp, cosy_cosp);

    trajectory_estimated_.at(index).covariance(5, 5) = yaw_cov * 10;

	return;
}
*/
