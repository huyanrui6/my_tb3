import rospy
from nav_msgs.msg import Odometry
import datetime

class OdomDataSaver:
    def __init__(self, interval):
        # Interval in seconds
        self.interval = interval
        self.last_data = None

        # Create or open a file to write data
        # self.file_name = "turtlebot_data_{}.txt".format(datetime.datetime.now().strftime("%Y%m%d_%H%M%S"))
        self.file_name = "turtlebot_data.txt"
        with open(self.file_name, 'w') as file:
            file.write("Time, X Coordinate, Y Coordinate, Linear Velocity, Angular Velocity\n")

        rospy.Subscriber("/odom", Odometry, self.odom_callback)
        rospy.Timer(rospy.Duration(self.interval), self.timer_callback)

    def odom_callback(self, data):
        self.last_data = data

    def timer_callback(self, event):
        if self.last_data:
            # Extracting data from the last received message
            now = rospy.get_time()
            linear_speed = self.last_data.twist.twist.linear.x
            angular_speed = self.last_data.twist.twist.angular.z
            x = self.last_data.pose.pose.position.x
            y = self.last_data.pose.pose.position.y

            # Writing data to file
            with open(self.file_name, 'a') as file:
                file.write("{}, {:.2f}, {:.2f}, {:.2f}, {:.2f}\n".format(now, x, y, linear_speed, angular_speed))

if __name__ == '__main__':
    rospy.init_node('odom_data_saver', anonymous=True)
    saver = OdomDataSaver(interval=0.5)  # Set the interval to 1 second
    rospy.spin()
