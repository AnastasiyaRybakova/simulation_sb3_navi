# silbot3_navigation_simulation

This package simulate silbot3 navigation in built-map with support omni-direction moving.

How to run:

#bringup silbot3 in gazebo world
+ roslaunch open_base gazebo\ description.launch 

#start silbot3 navigation and load configuration
+ roslaunch open_base silbot_gazebo.launch 

#start rviz configuration
+ roslaunch open_base rviz_gazebo.rviz

After that, we can test by sending 2D navigation 
