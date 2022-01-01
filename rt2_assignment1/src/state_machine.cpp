#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "rt2_assignment1/Command.h"
#include "rt2_assignment1/Position.h"
#include "rt2_assignment1/RandomPosition.h"
#include <rt2_assignment1/PlanningAction.h> 
#include <actionlib/client/simple_action_client.h> 
//#include <move_base_msgs/MoveBaseAction.h>
#include <rt2_assignment1/PlanningAction.h>
#include <actionlib/client/terminal_state.h>
#include <std_msgs/Float64.h>
#include <unistd.h>
#include <rt2_assignment1/PlanningAction.h>
bool start = false;
int stato = 0;
bool user_interface(rt2_assignment1::Command::Request &req, rt2_assignment1::Command::Response &res){
    if (req.command == "start"){
    	start = true;
		if (stato == 3)
			{
				stato = 0;
			}
		
    }
    if (req.command == "stop") {
    	start = false;
    }
    return true;
}
int main(int argc, char **argv)
{
   ros::init(argc, argv, "state_machine");
   ros::NodeHandle n;
   ros::ServiceServer service= n.advertiseService("/user_interface", user_interface);
   ros::ServiceClient client_rp = n.serviceClient<rt2_assignment1::RandomPosition>("/position_server");
    ros::Publisher ResetVelPub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
   geometry_msgs::Twist msg;
   actionlib::SimpleActionClient<rt2_assignment1::PlanningAction> client("/reaching_goal", true);
   rt2_assignment1::RandomPosition rp;
   rp.request.x_max = 5.0;
   rp.request.x_min = -5.0;
   rp.request.y_max = 5.0;
   rp.request.y_min = -5.0;
   rt2_assignment1::Position p;
   rt2_assignment1::PlanningResultConstPtr result;
   int res;
   while(ros::ok()){
		ros::spinOnce();
			if (start ){
				if (stato == 0){
					printf ("entro qui");
					client_rp.call(rp);
					client.waitForServer();
					rt2_assignment1::PlanningGoal goal;	
					goal.target_pose.header.frame_id = "base_link";
					goal.target_pose.header.stamp = ros::Time::now();
					goal.target_pose.pose.position.x = rp.response.x;
					goal.target_pose.pose.position.y = rp.response.y;
					goal.target_pose.pose.orientation.w = rp.response.theta;	
					std::cout << "\nGoing to the position: x= " << goal.target_pose.pose.position.x << " y= " << goal.target_pose.pose.position.y << " theta = " <<goal.target_pose.pose.orientation.w << std::endl;
					ROS_INFO("Sending goal");
					client.sendGoal(goal);
					stato = 1;
					//client.waitForResult();
				}

				if (stato == 1){
					if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
						ROS_INFO("Hooray, target reached!");
						stato = 0;
					}
					else{
						ROS_INFO("i'm reaching the goal");
					}
				}			
			}
		if (start == false and stato == 1 ){
			printf ("entro a cancellare");
			client.cancelGoal();
			msg.linear.x=0;
			msg.linear.y=0;
			msg.angular.z=0;
			ResetVelPub.publish(msg);
			stato = 3;
		}
	 }  
   return 0;
}
