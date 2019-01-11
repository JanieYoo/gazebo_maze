cmake_minimum_required(VERSION 2.8.3)
project(gazebo_maze)


find_package(catkin REQUIRED COMPONENTS
message_generation
roscpp
std_msgs
actionlib_msgs
actionlib
maze_action_msgs
nav_msgs
geometry_msgs
sensor_msgs
)

find_package(Boost REQUIRED COMPONENTS system)

add_service_files(
FILES
ReqRobotControl.srv
)


generate_messages(
DEPENDENCIES
std_msgs
actionlib_msgs
)


catkin_package(
LIBRARIES gazebo_maze
CATKIN_DEPENDS roscpp std_msgs actionlib_msgs actionlib maze_action_msgs nav_msgs geometry_msgs
sensor_msgs
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  )


add_executable(action_server src/action_server.cpp)
add_dependencies(action_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(action_server ${catkin_LIBRARIES})


add_executable(robot_controller src/service_server.cpp)
add_dependencies(robot_controller ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(robot_controller ${catkin_LIBRARIES})



