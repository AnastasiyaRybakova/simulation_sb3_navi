/*
 * JointSetup.h
 *
 *  Created on: Oct 12, 2015
 *      Author: cspark
 *      Modify: sjlee
 */

 #ifndef SRC_DEVICE_SERVER_ROBOT_SETUP_JOINTSETUP_H_
 #define SRC_DEVICE_SERVER_ROBOT_SETUP_JOINTSETUP_H_
 
 #include <ros/ros.h>
 #include <ros/console.h>
 #include <boost/thread.hpp>
 #include <sensor_msgs/JointState.h>
 #include <device/odif/arm/ArmERobot.h>
 #include <device/odif/pantilt/PanTiltERobot.h>
 #include <ros/package.h>
 #include <reactive/erobot/ERobotCore.h>
 #include <std_msgs/Float64.h>
 #include <std_srvs/Empty.h>
 
 using namespace std;
 using namespace ros;
 
 class JointSetup {
 
   private :
     double old_pantilt_data[2];
     double old_arms_data[6];
 
     double pantilt_data[2];
     double arms_data[6];
     double speed[8];
     double max_speed[8];
 
     double old_xtion_data;
     double xtion_data;
     double xtion_speed;
     double xtion_max_speed;
 
     bool isCall;
 
     CArm*					_armServerProxy2;
     CPanTilt*			_pantiltServerProxy2;
 
     ros::NodeHandle 	__node_handle;
 
     ros::Subscriber 	__head_pan_subscriber;
     ros::Subscriber 	__head_tilt_subscriber;
     ros::Subscriber 	__xtion_tilt_subscriber;
     ros::Subscriber 	__left_shoulder_pitch_subscriber;
     ros::Subscriber 	__left_shoulder_roll_subscriber;
     ros::Subscriber 	__left_elbow_roll_subscriber;
     ros::Subscriber 	__right_shoulder_pitch_subscriber;
     ros::Subscriber 	__right_shoulder_roll_subscriber;
     ros::Subscriber 	__right_elbow_roll_subscriber;
 
     ros::Subscriber 	__head_pan_speed_subscriber;
     ros::Subscriber 	__head_tilt_speed_subscriber;
     ros::Subscriber 	__xtion_tilt_speed_subscriber;
     ros::Subscriber 	__left_shoulder_pitch_speed_subscriber;
     ros::Subscriber 	__left_shoulder_roll_speed_subscriber;
     ros::Subscriber 	__left_elbow_roll_speed_subscriber;
     ros::Subscriber 	__right_shoulder_pitch_speed_subscriber;
     ros::Subscriber 	__right_shoulder_roll_speed_subscriber;
     ros::Subscriber 	__right_elbow_roll_speed_subscriber;
 
     ros::Publisher 		__joint_state_pub;
 
     ros::ServiceServer	_bodyServiceServer;
 
 
   private:
     void callbackHeadPan(const std_msgs::Float64 msg);
     void callbackHeadTilt(const std_msgs::Float64 msg);
     void callbackXtionTilt(const std_msgs::Float64 msg);
     void callbackLeftShoulderPitch(const std_msgs::Float64 msg);
     void callbackLeftShoulderRoll(const std_msgs::Float64 msg);
     void callbackLeftElbowRoll(const std_msgs::Float64 msg);
     void callbackRightShoulderPitch(const std_msgs::Float64 msg);
     void callbackRightShoulderRoll(const std_msgs::Float64 msg);
     void callbackRightElbowRoll(const std_msgs::Float64 msg);
 
     void callbackHeadPanSpeed(const std_msgs::Float64 msg);
     void callbackHeadTiltSpeed(const std_msgs::Float64 msg);
     void callbackXtionTiltSpeed(const std_msgs::Float64 msg);
     void callbackLeftShoulderPitchSpeed(const std_msgs::Float64 msg);
     void callbackLeftShoulderRollSpeed(const std_msgs::Float64 msg);
     void callbackLeftElbowRollSpeed(const std_msgs::Float64 msg);
     void callbackRightShoulderPitchSpeed(const std_msgs::Float64 msg);
     void callbackRightShoulderRollSpeed(const std_msgs::Float64 msg);
     void callbackRightElbowRollSpeed(const std_msgs::Float64 msg);
 
     void publishJointState();
 
     bool bodyDataServiceCallback(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response);
 
     double getRadian(double degree);
 
   public:
     JointSetup();
     virtual ~JointSetup();
     void moveJoint();
 
 };
 
 
 
 #endif /* SRC_DEVICE_SERVER_ROBOT_SETUP_JOINTSETUP_H_ */