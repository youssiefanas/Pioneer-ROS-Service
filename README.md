# Pioneer-ROS-Service
Moving Pioneer Robot to a goal on CoppeliaSim with ROS Service 

#Launching the node

## TurtleSim

- `roscore`  
- `rosrun my_turtle_service move_pioneer_server`  
- `rosrun my_turtle_service move_turtle_client "x" "y" "theta"`  

## Pioneer Service

- Lanch the coppeliaSim  
- `roscore`  
- `rosrun my_turtle_service move_pioneer_server /turtle1/cmd_vel:=/pioneer/cmd_vel`  
- `rosrun my_turtle_service move_turtle_client "x" "y" "theta"`  


# Demo

## TurtleSim Service to Goal



https://user-images.githubusercontent.com/121443735/215358252-b35ec43f-2ba8-4286-9f77-91aa09a234ca.mp4



## Pioneer Robot on CoppeliaSim Scene



https://user-images.githubusercontent.com/121443735/215358224-ed927d76-7df8-4d1d-ba3d-19bb31840130.mp4

