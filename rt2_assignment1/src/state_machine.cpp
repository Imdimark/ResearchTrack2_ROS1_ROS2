//#include "ros/ros.h"
#include "rt2_assignment1/srv/random_position.hpp"
#include "rt2_assignment1/srv/position.hpp"
#include "rt2_assignment1/srv/command.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include <chrono>
#include <cinttypes>
#include <memory>
#include <cstdlib>
#include <functional>
//#include "example_interfaces/srv/add_two_ints.hpp"
//using namespace std::chrono_literals;
//using AddTwoInts = example_interfaces::srv::AddTwoInts;

using service1 = rt2_assignment1::srv::Command;
using client1 = rt2_assignment1::srv::Position;
using client2 = rt2_assignment1::srv::RandomPosition;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace rt2_assignment1{

class FSM : public rclcpp::Node
{
public:

  FSM(const rclcpp::NodeOptions & options)
  : Node("finite_state_machine", options)
  {
    ////////////////////////////////////
     service_ui = this->create_service<service1>(
    "/user_interface", std::bind(&FSM::handle_service, this, _1, _2, _3));
    ////////////////////////////////////
     client_pos = this->create_client<client1>("go_to_point");
    
    while (!client_pos->wait_for_service(std::chrono::seconds(1))){
     if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "client interrupted while waiting for service to appear.");
      return;
    }
    }
   /* RCLCPP_INFO(this->get_logger(), "waiting for service to appear...");
  }
   timer_ = this->create_wall_timer(
      2000ms, std::bind(&MinimalClient::call_server, this));
  } */
    
     ////////////////////////////////////
     client_rand = this->create_client<client2>("position_server");
    
    while (!client_rand->wait_for_service(std::chrono::seconds(1))){
     if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "client interrupted while waiting for service to appear.");
      return;
    }
   /* RCLCPP_INFO(this->get_logger(), "waiting for service to appear...");
  }
   timer_ = this->create_wall_timer(
      2000ms, std::bind(&MinimalClient::call_server, this)); */
  
    
  }
  
  }
    
    //////////////////////////////////////////////////////    
    
    
    
    
    private:
    
    bool start = false;
    
    void finite_state_machine (){
    
                if (this->start){
	   	 auto req2 = std::make_shared<client2::Request>();
		
		 req2->x_max = 5.0;
		 req2->x_min = -5.0;
		 req2->y_max = 5.0;
		 req2->y_min = -5.0;
		 
		 
		 
		 
		 
		 
		 auto callback = [this](rclcpp::Client<client2>::SharedFuture future ) {
		         auto req = std::make_shared<client1::Request>();
		         this->response_rand=future.get();
		         
		         std::cout<<"received random numbers"<<std::endl;
		         std::cout<<this->response_rand->x<<std::endl;
		         
		         req->x = this->response_rand->x; //invia
		         req->y = this->response_rand->y;  //invia
		         req->theta = this->response_rand->theta; //invia
		         std::cout << "\nGoing to the position: x= " << req->x << " y= " <<req->y << " theta = " <<req->theta << std::endl; //reqx,y,theta = res2x,y,theta
		         auto response_received_callback = [this](rclcpp::Client<client1>::SharedFuture future) { //invio a go_to_point
		                                (void)future;
		                                std::cout << "Position reached" << std::endl;
		                                this->finite_state_machine();
            
                };
                
                auto resu = client_pos->async_send_request(req, response_received_callback);
           };
           auto future_result = client_rand->async_send_request(req2, callback);

	}
    }
    
    
    
    
    
    
    
    
    
    
    void handle_service(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<service1::Request> request,
        const std::shared_ptr<service1::Response> response)
        {
            (void)request_header;
            
             if (request->command == "start"){
  
    	         this->start = true; //setti start a true
    	         
    	         this->finite_state_machine();
              }
              else {
    	          this->start = false;
    	          
               }
             response->ok=true;  
             
         }
             
            /* RCLCPP_INFO(
             this->get_logger(),
              "request: %" PRId64 " + %" PRId64, request->a, request->b); */
              
             
              
          
    
    








/*


bool user_interface(rt2_assignment1::Command::Request &req, rt2_assignment1::Command::Response &res){
    if (req.command == "start"){
    	start = true;
    }
    else {
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
    ros::ServiceClient client_p = n.serviceClient<rt2_assignment1::Position>("/go_to_point");
   
   ros::ServiceServer service= n.advertiseService("/user_interface", user_interface);
   ros::ServiceClient client_rp = n.serviceClient<rt2_assignment1::RandomPosition>("/position_server");
   ros::ServiceClient client_p = n.serviceClient<rt2_assignment1::Position>("/go_to_point");
   
   rt2_assignment1::RandomPosition rp;
   rp.request.x_max = 5.0;
   rp.request.x_min = -5.0;
   rp.request.y_max = 5.0;
   rp.request.y_min = -5.0;
   rt2_assignment1::Position p;
   
   while(ros::ok()){
   	ros::spinOnce();
   	if (start){
   		client_rp.call(rp);
   		p.request.x = rp.response.x;
   		p.request.y = rp.response.y;
   		p.request.theta = rp.response.theta;
   		std::cout << "\nGoing to the position: x= " << p.request.x << " y= " <<p.request.y << " theta = " <<p.request.theta << std::endl;
   		client_p.call(p);
   		std::cout << "Position reached" << std::endl;
   	}
   }
   
   return 0;
}*/

    rclcpp::Service<service1>::SharedPtr service_ui; // per user interface 
    rclcpp::Client<client1>::SharedPtr client_pos; // service var: pointer to rclcpp service
    rclcpp::Client<client2>::SharedPtr client_rand; // service var: pointer to rclcpp service
    std::shared_ptr<client2::Response> response_rand;




   
};
}
RCLCPP_COMPONENTS_REGISTER_NODE(rt2_assignment1::FSM) 
