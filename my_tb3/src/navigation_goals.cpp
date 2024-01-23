#include "ros/ros.h"
#include "iostream"
#include "actionlib/client/simple_action_client.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "move_base_msgs/MoveBaseGoal.h"
#include "geometry_msgs/PoseStamped.h"

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "navigation_goals");
  ros::NodeHandle nh;
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);

  ros::Publisher cancel_pub = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 1);
  ros::Publisher goal_pose_pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1);

  move_base_msgs::MoveBaseGoal goal[4];
  actionlib_msgs::GoalID goalid;
  geometry_msgs::PoseStamped goal_pose;
  std::string state;
  goal_pose.header.frame_id = "map";

  /*依次填入先前纪录的四个目标点坐标*/
  goal_pose.pose.position.x = -1.6; //-0.3;
  goal_pose.pose.position.y = 0.65; //-0.5;
  goal_pose.pose.orientation.z = 0.68;
  goal_pose.pose.orientation.w = 1.0;
  goal[0].target_pose = goal_pose;
  goal_pose.pose.position.x = -1.36;
  goal_pose.pose.position.y = 4.0;
  goal_pose.pose.orientation.z = 0.7;
  goal_pose.pose.orientation.w = 0.8;
  goal[1].target_pose = goal_pose;
  goal_pose.pose.position.x = -8.7;
  goal_pose.pose.position.y = 5.5;
  goal_pose.pose.orientation.z = 1;
  goal_pose.pose.orientation.w = 0.0;
  goal[2].target_pose = goal_pose;
  goal_pose.pose.position.x = -1.86;
  goal_pose.pose.position.y = 3.9;
  goal_pose.pose.orientation.z = -0.7;
  goal_pose.pose.orientation.w = 0.35;
  goal[3].target_pose = goal_pose;
  ros::Rate loop(1);
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("waiting for move_base action server to come up");
  }
  for(int i=0; i<4; i++){
    goal[i].target_pose.header.stamp = ros::Time::now();
    ac.sendGoal(goal[i]);
    while((ac.getState() != actionlib::SimpleClientGoalState::SUCCEEDED)&&(ros::ok())){
       if(ac.getState() == actionlib::SimpleClientGoalState::ABORTED){
          ROS_INFO("aborted, start next goal");
          ac.cancelGoal();
          break;
        } 
    if(ac.getState() == actionlib::SimpleClientGoalState::REJECTED){
          ROS_INFO("error goal, maybe it is not in map; start next goal");
          cancel_pub.publish(goalid);
          break;     
        }
       state = ac.getState().toString();
       std::cout << state << std::endl;
       loop.sleep();
    }
    if((i<3)&&(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)){
       ROS_INFO("succecced, start next goal");
    }
  }
  ROS_INFO("all goal is sended");
  return 0;
}
