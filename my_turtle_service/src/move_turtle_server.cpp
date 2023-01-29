#include <iostream> 
#include <bits/stdc++.h>
#include <cstdlib>
#include <math.h>
#include <chrono>
#include "ros/ros.h" 
#include "geometry_msgs/Twist.h" 
#include "geometry_msgs/Pose2D.h"
#include "turtlesim/Pose.h" 
#include "my_turtle_service/goal.h"

using namespace std;

geometry_msgs::Twist turtle_vel;
geometry_msgs::Pose2D turtle_pose;
ros::Publisher velocityPub;

float desired_x, desired_y, desired_theta, dist_err, theta_err;
const float PI = 3.141592654;
const float K_l = 0.4;
const float K_a = 1.5;
const float distanceTolerance = 0.1;
const float angleTolerance = 0.001;


// turtlesim callback
void update_turtle_pose(const turtlesim::PoseConstPtr &currentPose) {
    turtle_pose.x = currentPose->x;
    turtle_pose.y = currentPose->y;
    turtle_pose.theta = currentPose->theta;
    ROS_INFO("x Pose [%f] y pose [%f] theta pose [%f]", (float) turtle_pose.x, (float) turtle_pose.y, (float)turtle_pose.theta);
}




bool get_pose(my_turtle_service::goal::Request &req, my_turtle_service::goal::Response &res){
    ros::Rate rate(10); 
    ros::NodeHandle node;
    
    desired_x=req.x;
    desired_y=req.y;
    desired_theta=req.theta;
    ros::Subscriber turtle_pose_sub = node.subscribe("turtle1/pose", 0, update_turtle_pose);

    ros::spinOnce();
    ROS_INFO("request: x=%f, y=%f, theta=%f", (float)req.x, (float)req.y, (float)req.theta);
    auto start = chrono::steady_clock::now();
    velocityPub = node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 0);
    

 dist_err = sqrt(pow(desired_x - turtle_pose.x , 2) + pow(desired_y - turtle_pose.y, 2));
    while (dist_err > distanceTolerance){
    ros::Subscriber turtle_pose_sub = node.subscribe("turtle1/pose", 0, update_turtle_pose);

    ros::spinOnce();

    dist_err = sqrt(pow(desired_x - turtle_pose.x , 2) + pow(desired_y - turtle_pose.y, 2));
    theta_err=desired_theta-turtle_pose.theta;
    float alpha = atan2((desired_y - turtle_pose.y),(desired_x - turtle_pose.x))-turtle_pose.theta;
    alpha = fmod((alpha+ PI), (2*PI))-PI;
    turtle_vel.linear.x=dist_err*K_l;
    turtle_vel.angular.z = K_a*alpha;
    velocityPub.publish(turtle_vel);
    // ROS_INFO("alpha [%f]",(float) alpha);
    // ROS_INFO("velocity: t vel x=%f, ang z=%f", (float)turtle_vel.linear.x , (float)turtle_vel.angular.z);
    // ROS_INFO("turtle_err => x = [%f], theta = [%f]", (float) dist_err, (float) theta_err);
    rate.sleep();
    }
    ROS_INFO("goal reached");

    while (abs(theta_err)>angleTolerance){
    ros::Subscriber turtle_pose_sub = node.subscribe("turtle1/pose", 0, update_turtle_pose);

    ros::spinOnce();
    theta_err=desired_theta-turtle_pose.theta;
    cout<<turtle_pose.theta<<endl;
    turtle_vel.linear.x=0;
    turtle_vel.angular.z = K_a*theta_err;
    velocityPub.publish(turtle_vel);
    rate.sleep();
    }
    ROS_INFO("Pose reached");
    auto end = chrono::steady_clock::now();
    
    res.time=chrono::duration_cast<chrono::seconds>(end - start).count();
    ROS_INFO("sending back response: [%f]", (float)res.time);
    return true;

}

int main(int argc , char **argv){
    ros::init(argc, argv, "turtle_run");
    ros::NodeHandle node;
    ros::ServiceServer service = node.advertiseService("Turtle_service",get_pose);
    ros::Subscriber turtle_pose_sub = node.subscribe("turtle1/pose", 0, update_turtle_pose);


    ROS_INFO("Ready To Go: ");
    ros::spin();

  return 0;
}