#! /usr/bin/env python
import rospy
from nav_msgs.msg import Odometry
# import datetime

# Create or open a file to write data
file_name = "turtlebot_data.txt"
with open(file_name, 'w') as file:
    file.write("Time, X Coordinate, Y Coordinate, Linear Velocity, Angular Velocity\n")

def odom_callback(data):
    # Get current time
    now = rospy.get_time()

    # Extract velocity and position information from odometry message
    linear_speed = data.twist.twist.linear.x
    angular_speed = data.twist.twist.angular.z
    x = data.pose.pose.position.x
    y = data.pose.pose.position.y

    print("position x={:.2f}, y={:.2f}".format(x, y))
    print("linear_speed: {:.2f}, angular_speed: {:.2f}".format(linear_speed, angular_speed))

    # Write data to file
    with open(file_name, 'a') as file:
        file.write("{}, {:.2f}, {:.2f}, {:.2f}, {:.2f}\n".format(now, x, y, linear_speed, angular_speed))

def listener():
    rospy.init_node('odom_data_saver', anonymous=True)
    rospy.Subscriber("/odom", Odometry, odom_callback)
    rospy.spin()

if __name__ == '__main__':
    listener()