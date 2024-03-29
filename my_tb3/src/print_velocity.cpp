#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "iostream"
 
void callback(const geometry_msgs::Twist& cmd_vel)
{
    ROS_INFO("Received a /cmd_vel message!");
    ROS_INFO("Linear Components:[%f,%f,%f]",cmd_vel.linear.x,cmd_vel.linear.y,cmd_vel.linear.z);
    ROS_INFO("Angular Components:[%f,%f,%f]",cmd_vel.angular.x,cmd_vel.angular.y,cmd_vel.angular.z);
}
 
int main(int argc, char** argv)
{
    ros::init(argc, argv, "cmd_vel_listener");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("cmd_vel", 1000, callback); 
    //我的turtlebot3上面速度话题是cmd_vel，若读者不同，自己改动

    //查看节点可以在终端运行：$rqt_graph     会画出当前你的小车运行的所有节点命名
    ros::spin();

    return 1;
}