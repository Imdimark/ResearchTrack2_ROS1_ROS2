#! /usr/bin/env python


import rospy
import time
from rt2_assignment1.srv import Command
import roslib
roslib.load_manifest('rt2_assignment1')
import actionlib
from rt2_assignment1.msg import PlanningAction





def reset_vel():
	twist_msg = Twist()
    twist_msg.linear.x = 0
    twist_msg.linear.y = 0
    twist_msg.angular.theta = 0
    pub.publish(twist_msg)
	
	
	
	
def main():
    rospy.init_node('user_interface')
    ui_client = rospy.ServiceProxy('/user_interface', Command)
    pub_vel_reset = rospy.Publisher('/cmd_vel', Twist,queue_size=1)
    client_reset = actionlib.SimpleActionClient('planning', PlanningAction)
    time.sleep(10)
    rate = rospy.Rate(20)
    x = int(input("\nPress 1 to start the robot "))
    while not rospy.is_shutdown():
        if (x == 1):
            ui_client("start")
            x = int(input("\nPress 0 to stop the robot "))
        
        else:
        	print("I'm cancelling all goals")
        	client_reset.cancell_all_goals()
        	reset_vel()
              	
        	
            """print("Please wait, the robot is going to stop when the position will be reached")"""
            ui_client("stop")
            
            
            x = int(input("\nPress 1 to start the robot "))
            
if __name__ == '__main__':
    main()
