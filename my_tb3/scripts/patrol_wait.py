#!/usr/bin/env python

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import time

def move_to_goal(x, y):
    # create client
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    # set goal
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation.w = 1.0

    client.send_goal(goal)
    wait = client.wait_for_result()
    
    if not wait:
        rospy.logerr("Action server not available!")
        rospy.signal_shutdown("Action server not available!")
    else:
        return client.get_result()

def patrol():
    
    rospy.init_node('turtlebot_patrol', anonymous=True)

    points = [[0, 0], [1, 1], [2, 2], [3, 3]] 
    while not rospy.is_shutdown():
        for point in points:
            rospy.loginfo("Moving to: x = %f, y = %f" % (point[0], point[1]))
            move_to_goal(point[0], point[1])
            rospy.loginfo("Reached point, waiting for 2 seconds.")
            time.sleep(2) # wait 2s

if __name__ == '__main__':
    try:
        patrol()
    except rospy.ROSInterruptException:
        pass
