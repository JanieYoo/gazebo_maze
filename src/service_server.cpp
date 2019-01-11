#include "ros/ros.h"                          
#include "gazebo_maze/ReqRobotControl.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
class MazeController{

private:
    ros::ServiceServer maze_server_;
    ros::NodeHandle nh_;

    ros::Publisher cmd_pub_;
    ros::Subscriber laser_sub_;
    
    double linear_x_;
    double angular_z_;

    double angular_param_;

public:
    MazeController()
    : linear_x_(0.0), angular_z_(0.0)
    { 
        ROS_INFO("ready maze controller service server!");
        maze_server_= nh_.advertiseService("req_robot_control",&MazeController::serviceCB, this);
        cmd_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
        laser_sub_ = nh_.subscribe("/scan", 1, &MazeController::laserCB, this);
        
	}	


    void laserCB(const sensor_msgs::LaserScan::ConstPtr& msg)
    {

    nh_.param("angular_param", angular_param_, 1.0);
     
    float front = msg->ranges[0];
    float left = msg->ranges[90];
    float right = msg->ranges[270];
    float total = right - left ;
        
    linear_x_ =0.8;
    angular_z_= total*angular_param_ ;
    }

    bool serviceCB(gazebo_maze::ReqRobotControl::Request &req,
                     gazebo_maze::ReqRobotControl::Response &res)
    {
        geometry_msgs::Twist control_msg;

        control_msg.linear.x = linear_x_;
        control_msg.angular.z = angular_z_;

        cmd_pub_.publish(control_msg);

        ROS_INFO("cmd_vel published! linear.x: %f, angular.z: %f", control_msg.linear.x, control_msg.angular.z);

        res.success = true;

        return true;
    }

};


int main(int argc, char **argv)              
{
  ros::init(argc, argv, "service_server");   

  MazeController maze_controller;

  ros::spin();   

  return 0;
}
