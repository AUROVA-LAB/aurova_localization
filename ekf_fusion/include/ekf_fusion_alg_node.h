// Copyright (C) 2010-2011 Institut de Robotica i Informatica Industrial, CSIC-UPC.
// Author 
// All rights reserved.
//
// This file is part of iri-ros-pkg
// iri-ros-pkg is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// IMPORTANT NOTE: This code has been generated through a script from the 
// iri_ros_scripts. Please do NOT delete any comments to guarantee the correctness
// of the scripts. ROS topics can be easly add by using those scripts. Please
// refer to the IRI wiki page for more information:
// http://wikiri.upc.es/index.php/Robotics_Lab
/**
 * \file ekf_fusion_alg_node.h
 *
 *  Created on: 21 Nov 2018
 *      Author: m.a.munoz
 */

#ifndef _ekf_fusion_alg_node_h_
#define _ekf_fusion_alg_node_h_

#include <iri_base_algorithm/iri_base_algorithm.h>
#include "ekf_fusion_alg.h"

// [publisher subscriber headers]

// [service client headers]

// [action server client headers]

/**
 * \brief IRI ROS Specific Algorithm Class
 *
 */
class EkfFusionAlgNode : public algorithm_base::IriBaseAlgorithm<EkfFusionAlgorithm>
{
private:

  ekf::KalmanConfiguration kalman_config_;
  CEkfPtr ekf_;
  bool flag_plot_pose_;
  bool is_simulation_;
  std::string frame_id_;
  std::string child_id_;
  float min_speed_;
  geometry_msgs::PoseWithCovarianceStamped plot_pose_;
  geometry_msgs::PoseWithCovarianceStamped init_pose_;
  geometry_msgs::TransformStamped odom_to_map_;
  tf::TransformBroadcaster broadcaster_;
  tf::TransformListener listener_;

  // [publisher attributes]
  ros::Publisher plot_pose_pub_;

  // [subscriber attributes]
  ros::Subscriber odom_gps_sub_;
  ros::Subscriber odom_raw_sub_;
  ros::Subscriber init_pose_sub_;
  
  /**
   * \brief callback for read initial pose messages
   * This message can be read from different localization sources by remapping in the
   * execution of the node.
   */
  void cb_getInitPoseMsg(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& init_msg);

  /**
   * \brief callback for read odometry messages
   * This message can be read from different localization sources by remapping in the
   * execution of the node.
   */
  void cb_getGpsOdomMsg(const nav_msgs::Odometry::ConstPtr& odom_msg);

  /**
   * \brief callback for read odometry messages
   * This message can be read from different localization sources by remapping in the
   * execution of the node.
   */
  void cb_getRawOdomMsg(const nav_msgs::Odometry::ConstPtr& odom_msg);

  // [service attributes]

  // [client attributes]

  // [action server attributes]

  // [action client attributes]

  /**
   * \brief config variable
   *
   * This variable has all the driver parameters defined in the cfg config file.
   * Is updated everytime function config_update() is called.
   */
  Config config_;
public:
  /**
   * \brief Constructor
   *
   * This constructor initializes specific class attributes and all ROS
   * communications variables to enable message exchange.
   */
  EkfFusionAlgNode(void);

  /**
   * \brief Destructor
   *
   * This destructor frees all necessary dynamic memory allocated within this
   * this class.
   */
  ~EkfFusionAlgNode(void);

protected:
  /**
   * \brief main node thread
   *
   * This is the main thread node function. Code written here will be executed
   * in every node loop while the algorithm is on running state. Loop frequency
   * can be tuned by modifying loop_rate attribute.
   *
   * Here data related to the process loop or to ROS topics (mainly data structs
   * related to the MSG and SRV files) must be updated. ROS publisher objects
   * must publish their data in this process. ROS client servers may also
   * request data to the corresponding server topics.
   */
  void mainNodeThread(void);

  /**
   * \brief dynamic reconfigure server callback
   *
   * This method is called whenever a new configuration is received through
   * the dynamic reconfigure. The derivated generic algorithm class must
   * implement it.
   *
   * \param config an object with new configuration from all algorithm
   *               parameters defined in the config file.
   * \param level  integer referring the level in which the configuration
   *               has been changed.
   */
  void node_config_update(Config &config, uint32_t level);

  /**
   * \brief node add diagnostics
   *
   * In this abstract function additional ROS diagnostics applied to the
   * specific algorithms may be added.
   */
  void addNodeDiagnostics(void);

  // [diagnostic functions]

  // [test functions]
};

#endif
