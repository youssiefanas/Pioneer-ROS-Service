#include <iostream> 
#include <bits/stdc++.h>
#include <cstdlib>
#include <math.h>
#include "ros/ros.h" 
#include "geometry_msgs/Twist.h" 
#include "geometry_msgs/Pose2D.h"
#include "turtlesim/Pose.h" 
#include "my_turtle_service/goal.h"

using namespace std;

int main(int argc, char **argv){
    ros::init(argc, argv, "move_turtle_client");
    if (argc!=4){
        ROS_INFO("Enter X Y Theta: ");
        return 1;
    }
    ros::NodeHandle node;
    ros::ServiceClient client = node.serviceClient<my_turtle_service::goal>("Turtle_service");
    my_turtle_service::goal srv;
    srv.request.x = atof(argv[1]);
    srv.request.y = atof(argv[2]);
    srv.request.theta = atof(argv[3]);
    if (client.call(srv)){
         ROS_INFO("Reached in time: %f", (float)srv.response.time);
    }
    else{
        ROS_INFO("Service Faild");
        return 1;
    }
    return 0;
}