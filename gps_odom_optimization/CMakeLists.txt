cmake_minimum_required(VERSION 2.8.3)
project(gps_odom_optimization)

## Find catkin macros and libraries
find_package(catkin REQUIRED)
FIND_PACKAGE(Eigen3 REQUIRED)
# ******************************************************************** 
#                 Add catkin additional components here
# ******************************************************************** 
find_package(catkin REQUIRED COMPONENTS iri_base_algorithm tf nav_msgs geometry_msgs eigen_conversions tf_conversions)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

# ******************************************************************** 
#           Add system and labrobotica dependencies here
# ******************************************************************** 
# find_package(<dependency> REQUIRED)
find_package(Ceres REQUIRED)

# ******************************************************************** 
#           Add topic, service and action definition here
# ******************************************************************** 
## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

# ******************************************************************** 
#                 Add the dynamic reconfigure file 
# ******************************************************************** 
generate_dynamic_reconfigure_options(cfg/GpsOdomOptimization.cfg)

# ******************************************************************** 
#                 Add run time dependencies here
# ******************************************************************** 
catkin_package(
#  INCLUDE_DIRS 
#  LIBRARIES 
# ******************************************************************** 
#            Add ROS and IRI ROS run time dependencies
# ******************************************************************** 
 CATKIN_DEPENDS iri_base_algorithm tf nav_msgs geometry_msgs
# ******************************************************************** 
#      Add system and labrobotica run time dependencies here
# ******************************************************************** 
  DEPENDS Ceres_LIBRARIES
)

###########
## Build ##
###########

# ******************************************************************** 
#                   Add the include directories 
# ******************************************************************** 
include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS}) 

link_directories(
  include
  ${PCL_LIBRARY_DIRS}
  ${CERES_LIBRARY_DIRS}
)

# include_directories(${<dependency>_INCLUDE_DIRS})

## Declare a cpp library
# add_library(${PROJECT_NAME} <list of source files>)

## Declare a cpp executable
add_executable(${PROJECT_NAME} src/gps_odom_optimization_alg.cpp src/gps_odom_optimization_alg_node.cpp
                               include/common_types.hpp include/ceres_structs.hpp
                               include/optimization_process.hpp)

# ******************************************************************** 
#                   Add the libraries
# ******************************************************************** 
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${CERES_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} ${<dependency>_LIBRARIES})

# ******************************************************************** 
#               Add message headers dependencies 
# ******************************************************************** 
# add_dependencies(${PROJECT_NAME} <msg_package_name>_generate_messages_cpp)
add_dependencies(${PROJECT_NAME} tf_generate_messages_cpp)
add_dependencies(${PROJECT_NAME} nav_msgs_generate_messages_cpp)
add_dependencies(${PROJECT_NAME} geometry_msgs_generate_messages_cpp)
# ******************************************************************** 
#               Add dynamic reconfigure dependencies 
# ******************************************************************** 
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS})
