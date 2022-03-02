#! /usr/bin/env python
"""
.. module:: go_to_point
    :platform: Unix
    :synopsis: this python code implement the interface with the user, it accepts imput from keybord or jupyter
.. moduleauthor:: Di Marco Giovanni
This node, go_to_point, is the node that implements the phisical behaviour of the robot.
It receive from one side the target position, by the goal,
Subscribes to:
None
Publishes to:
None
Service :
/user_interface
"""
import rospy
from geometry_msgs.msg import Twist, Point, Pose
from sensor_msgs.msg import LaserScan
from nav_msgs.msg import Odometry
from tf import transformations
from rt2_assignment1.srv import Position
import math
import actionlib
import actionlib.msg
import rt2_assignment1.msg

position_ = Point()
pose_ = Pose()
yaw_ = 0

position_ = 0
state_ = 0
#sub_odom = None
pub_ = None
server = None
desired_position = Point()
desired_position.z = 0
# parameters for control
yaw_precision_ = math.pi / 9  # +/- 20 degree allowed
yaw_precision_2_ = math.pi / 90  # +/- 2 degree allowed
dist_precision_ = 0.1
kp_a = -3.0 
kp_d = 0.2
ub_a = 0.6
lb_a = -0.5
ub_d = 0.6

def clbk_odom(msg):
    '''
    Description of the callback:
    Basically the quaternion of the position, retrived from the /odom subscriber is converted to euler one.
    The position is stored in the position_ variable, it is declared as Point()
    Args:
      msg(Odometry): data retrieved by */odom* topic
    Returns:
      None
    '''
    global position_, pose_
    global yaw_
    position_ = msg.pose.pose.position
    pose_ = msg.pose.pose
    quaternion = (
        msg.pose.pose.orientation.x,
        msg.pose.pose.orientation.y,
        msg.pose.pose.orientation.z,
        msg.pose.pose.orientation.w)
    euler = transformations.euler_from_quaternion(quaternion)
    yaw_ = euler[2]

def change_state(state):
    '''
    Description of the change_state function:
    This function assign sta value state to the global variable (*state_*) 
    Args:
      state(int): the state of the robot
    Returns:
      None
    '''
    global state_
    state_ = state
    print ('State changed to [%s]' % state_)

def normalize_angle(angle):
    '''
    Description of the normalize_angle function:
    This function normalize the value of the angle
    Args:
      angle(float): value of the angle not normalized
    Returns:
      angle(float): value of the angle normalized
    '''
    if(math.fabs(angle) > math.pi):
        angle = angle - (2 * math.pi * angle) / (math.fabs(angle))
    return angle

def fix_yaw(des_pos):
    '''
    Description of the fix_yaw function:
    
    This function set the angular velocity such that the orientation
    of the robot allow the reaching of the target
		   
    Args:
      des_yaw:  the expected position
    Returns:
      None
    '''
    global yaw_precision_2_, yaw, pub, state_
    desired_yaw = math.atan2(des_pos.y - position_.y, des_pos.x - position_.x)
    err_yaw = normalize_angle(desired_yaw - yaw_)
    rospy.loginfo(err_yaw)
    twist_msg = Twist()
    if math.fabs(err_yaw) > yaw_precision_2_:
        twist_msg.angular.z = kp_a*err_yaw
        if twist_msg.angular.z > ub_a:
            twist_msg.angular.z = ub_a
        elif twist_msg.angular.z < lb_a:
            twist_msg.angular.z = lb_a
    pub_.publish(twist_msg)
    # state change conditions
    if math.fabs(err_yaw) <= yaw_precision_2_:
        print ('Yaw error: [%s]' % err_yaw)
        change_state(1)


def go_straight_ahead(des_pos):
    '''
    Description of the go_straight_ahead function:

    This function computes the robot trajectory error in the orientation and
    in the position in order to set, using a threshold, the most suitable angular velocity and 
    the linear velocity, which is already set.
		   
    Args:
      des_pos(Point): the desired position, both x and y
    Returns:
      None
    '''
    global yaw_precision_, yaw, pub, state_
    desired_yaw = math.atan2(des_pos.y - position_.y, des_pos.x - position_.x)
    err_yaw = desired_yaw - yaw_
    err_pos = math.sqrt(pow(des_pos.y - position_.y, 2) +
                        pow(des_pos.x - position_.x, 2))
    err_yaw = normalize_angle(desired_yaw - yaw_)
    rospy.loginfo(err_yaw)

    if err_pos > dist_precision_:
        twist_msg = Twist()
        twist_msg.linear.x = 0.3
        if twist_msg.linear.x > ub_d:
            twist_msg.linear.x = ub_d

        twist_msg.angular.z = kp_a*err_yaw
        pub_.publish(twist_msg)
    else: # state change conditions
        print ('Position error: [%s]' % err_pos)
        change_state(2)

    # state change conditions
    if math.fabs(err_yaw) > yaw_precision_:
        print ('Yaw error: [%s]' % err_yaw)
        change_state(0)

def fix_final_yaw(des_yaw):
    '''
    Description of the fix_final_yaw function:
    
    This function set the angular velocity such that the orientation
    of the robot allow the reaching of the target
		   
    Args:
      des_yaw:  the expected angle
    Returns:
      None
    '''

    global yaw_precision_2_, yaw, pub, state_
    err_yaw = normalize_angle(des_yaw - yaw_)
    rospy.loginfo(err_yaw)
    twist_msg = Twist()
    if math.fabs(err_yaw) > yaw_precision_2_:
        twist_msg.angular.z = kp_a*err_yaw
        if twist_msg.angular.z > ub_a:
            twist_msg.angular.z = ub_a
        elif twist_msg.angular.z < lb_a:
            twist_msg.angular.z = lb_a
    pub_.publish(twist_msg)
    # state change conditions
    if math.fabs(err_yaw) <= yaw_precision_2_:
        #print ('Yaw error: [%s]' % err_yaw)
        change_state(3)
        
def done():
    """
    Description of done function:
		    
    reset speeds, as the name suggests, when the goal is reached
    
    Args :
      None
    
    Returns :
      None
          
    """
    twist_msg = Twist()
    twist_msg.linear.x = 0
    twist_msg.angular.z = 0
    pub_.publish(twist_msg)
    
#def go_to_point(req):
def go_to_point(goal):
    
    """
    Description of go_to_point function:
		    
    This function takes in input the goal and assures that the target is reached. When it appens, the fuction prints "Goal: Succeeded!"
    
    Args :
      goal (Float) = the target to reach
    
    
    Returns :
      None
          
    """
    
    global desired_position, server, state_
    rate = rospy.Rate(20)
    desired_position.x = goal.target_pose.pose.position.x
    desired_position.y = goal.target_pose.pose.position.y
    #desired_position.z= goal.target_pose.pose.orientation.w
    state_ = 0
    #change_state(0)
    #success = True
    
    feedback = rt2_assignment1.msg.PlanningFeedback()
    result = rt2_assignment1.msg.PlanningResult()
    while not rospy.is_shutdown():
        if server.is_preempt_requested():
            rospy.loginfo('Goal was preempted')
            server.set_preempted()
            success = False
            break
        elif state_ == 0:
            feedback.stat = "Fixing the yaw"
            feedback.actual_pose = pose_
            server.publish_feedback(feedback)
            fix_yaw(desired_position)
        elif state_ == 1:
            feedback.stat = "Angle aligned"
            feedback.actual_pose = pose_
            server.publish_feedback(feedback)
            go_straight_ahead(desired_position)
        elif state_ == 2:
            feedback.stat = "Target reached!"
            feedback.actual_pose = pose_
            server.publish_feedback(feedback)
            success = True
            done()
            break
        else:
            rospy.logerr('Unknown state!')

        rate.sleep()
    if success:
        rospy.loginfo('Goal: Succeeded!')
        server.set_succeeded(result)

    """while True:
    	if state_ == 0:
    	    fix_yaw(desired_position)
    	elif state_ == 1:
    	    go_straight_ahead(desired_position)
    	elif state_ == 2:
    	    fix_final_yaw(des_yaw)
    	elif state_ == 3:
    	    done()
    	    break
    return True"""

    """
    Main function, description:

    In the main function there are all the initializations:
    1) the node  'go_to_point' 
    2) the publisher to the topic /cmd_vel
    3) thesubscriber to the topic /odom
    4) the server of the /reaching_goal action
           
    Args :
      None
    
    Returns :
      None
             
    """
def main():
    global pub_, server
    rospy.init_node('go_to_point')
    
    pub_ = rospy.Publisher('/cmd_vel', Twist, queue_size=1)
    sub_odom = rospy.Subscriber('/odom', Odometry, clbk_odom)
    server = actionlib.SimpleActionServer('/reaching_goal', rt2_assignment1.msg.PlanningAction, go_to_point, auto_start=False)  
    
    server.start()
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
        rate.sleep()
    #rospy.spin()

if __name__ == '__main__':
    main()
