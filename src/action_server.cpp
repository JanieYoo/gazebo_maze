#include <ros/ros.h>                              
#include <actionlib/server/simple_action_server.h>
#include <gazebo_maze/ReqRobotControl.h>
#include "nav_msgs/Odometry.h"
#include "maze_action_msgs/MazeProjectAction.h"

class Maze{
protected:

  ros::NodeHandle nh_; 
  actionlib::SimpleActionServer<maze_action_msgs::MazeProjectAction> as_;
  std::string action_name_;
  ros::ServiceClient maze_client_; 
 
  maze_action_msgs::MazeProjectFeedback feedback_;
  maze_action_msgs::MazeProjectResult result_;
  ros::Subscriber odom_sub_;

  double x_;
  double y_;

public:
  Maze(std::string name) :
    as_(nh_, name, boost::bind(&Maze::executeCB, this, _1), false),
    action_name_(name), x_(0.0), y_(0.0)
  {
    as_.start();
    maze_client_ = nh_.serviceClient<gazebo_maze::ReqRobotControl>("req_robot_control");
    odom_sub_ = nh_.subscribe("/odom", 1, &Maze::odomCB, this);
  }

  ~Maze(void)
  {
  }

  void odomCB(const nav_msgs::Odometry::ConstPtr& msg)
    {
        x_ = msg->pose.pose.position.x;
        y_ = msg->pose.pose.position.y;
    }


  void executeCB(const maze_action_msgs::MazeProjectGoalConstPtr &goal)
  { 
	gazebo_maze::ReqRobotControl req;

while(ros::ok()){

    //TODO: make later
    if(0)
    {
        as_.setSucceeded(result_);
    }

    maze_client_.call(req);

    feedback_.odometry.pose.pose.position.x = x_;
    feedback_.odometry.pose.pose.position.y = y_;
    
      if (as_.isPreemptRequested() || !ros::ok())
      {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        as_.setPreempted();
        break;
      }

     
      as_.publishFeedback(feedback_);
     
   
  }

}
};

int main(int argc, char** argv)                     
{
  ros::init(argc, argv, "action_server");           
  Maze maze("maze_project"); 
  ros::spin();                                      
  return 0;
}
