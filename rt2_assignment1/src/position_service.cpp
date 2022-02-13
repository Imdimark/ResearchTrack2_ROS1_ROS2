//#include "ros/ros.h"
#include <inttypes.h>
#include <memory>
#include <functional>
#include "rt2_assignment1/srv/random_position.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"



using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using service3 = rt2_assignment1::srv::RandomPosition;


namespace rt2_assignment1{

class Position_Service : public rclcpp::Node
{

    public:

      Position_Service(const rclcpp::NodeOptions & options)
      : Node("servpos", options)
      {
           service_rand = this->create_service<service3>("/position_server", std::bind(&Position_Service::handle_service, this, _1, _2, _3)); 
      }

    private:
  
    void handle_service(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<service3::Request> request,
    const std::shared_ptr<service3::Response> response)
    {
        (void)request_header;
        
        response->x = request->x_min + (rand() / ( RAND_MAX / (request->x_max-request->x_min) ) );
        response->y = request->y_min + (rand() / ( RAND_MAX / (request->y_max-request->y_min) ) );
        response->theta =  -3.14+ (rand() / ( RAND_MAX / (6.28)));
    
      
    }
    rclcpp::Service<service3>::SharedPtr service_rand; 

};
}
RCLCPP_COMPONENTS_REGISTER_NODE(rt2_assignment1::Position_Service)
