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

#ifndef _geo_localization_alg_node_h_
#define _geo_localization_alg_node_h_

#include <iri_base_algorithm/iri_base_algorithm.h>
#include <localization/data_processing.h>
#include <localization/optimization_process.h>
#include <localization/latlong_utm.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float64.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include <pcl_ros/point_cloud.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <tf/tf.h>
#include <iostream>
#include <fstream>
#include <pcl/registration/icp.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <eigen_conversions/eigen_msg.h>
#include <tf_conversions/tf_eigen.h>
#include "geo_localization_alg.h"

// [publisher subscriber headers]

// [service client headers]

// [action server client headers]

/**
 * \brief IRI ROS Specific Algorithm Class
 *
 */
class GeoLocalizationAlgNode : public algorithm_base::IriBaseAlgorithm<GeoLocalizationAlgorithm>
{
  private:
  
    double lat_zero_;
    double lon_zero_;
    float offset_map_x_;
    float offset_map_y_;
    int count_;
    int margin_asso_constraints_;
    int margin_gnss_constraints_;
    double asso_weight_;
    double odom_weight_;
    double asso_preweight_;
    float margin_gnss_distance_;
    bool flag_gps_corr_;
    bool save_data_;
    bool save_map_;
    bool ground_truth_;
    int gt_last_frame_;
    std::vector<double> gt_key_frames_;
    std::string out_data_;
    std::string out_map_;
    std::string out_gt_;
    std::string world_id_;
    std::string map_id_;
    std::string odom_id_;
    std::string base_id_;
    std::string lidar_id_;
    pcl::PointCloud<pcl::PointXYZ> last_detect_pcl_;
    data_processing::ConfigParams data_config_;
    data_processing::PolylineMap map_;
    data_processing::DataProcessing *data_;
    optimization_process::OptimizationProcess *optimization_;
    optimization_process::ConfigParams optimization_config_;
    geometry_msgs::TransformStamped tf_to_utm_;
    geometry_msgs::TransformStamped tf_to_map_;
    tf::TransformBroadcaster broadcaster_;
    tf::TransformListener listener_;
    visualization_msgs::MarkerArray marker_array_;
    
    // [publisher attributes]
    ros::Publisher marker_pub_;
    ros::Publisher localization_publisher_;
    ros::Publisher landmarks_publisher_;
    ros::Publisher detection_publisher_;
    ros::Publisher corregist_publisher_;
    ros::Publisher gpscorrected_publisher_;
    ros::Publisher wa_publisher_;
    nav_msgs::Odometry localization_msg_;

    // [subscriber attributes]
    ros::Subscriber odom_subscriber_;
    ros::Subscriber gnss_subscriber_;
    ros::Subscriber detc_subscriber_;

    void odom_callback(const nav_msgs::Odometry::ConstPtr& msg);
    void gnss_callback(const nav_msgs::Odometry::ConstPtr& msg);
    void detc_callback(const sensor_msgs::PointCloud2::ConstPtr &msg);

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
    GeoLocalizationAlgNode(void);

   /**
    * \brief Destructor
    * 
    * This destructor frees all necessary dynamic memory allocated within this
    * this class.
    */
    ~GeoLocalizationAlgNode(void);

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

    //// NEW LOCAL FUNCTIONS
    void fromUtmTransform(void);
    void mapToOdomInit(void);
    int parseMapToRosMarker(visualization_msgs::MarkerArray& marker_array);
    void computeOptimizationProblem (void);
    void computeOptimizationProblemGT (void);

    // [diagnostic functions]
    
    // [test functions]
};

#endif
