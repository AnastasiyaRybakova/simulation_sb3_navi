/*
 * JointSetup.cpp
 *
 *  Created on: Oct 12, 2015
 *      Author: cspark
 *		Modify: sjlee
 */

 #include "JointSetup.h"
 #include <kdl_parser/kdl_parser.hpp>
 #include <kdl/frames_io.hpp>
 #include <geometry_msgs/TransformStamped.h>
 #include <tf2_kdl/tf2_kdl.h>
 
JointSetup::JointSetup(){
 
	//cout << "\n\n\n\n Hello! JointSetup Start... \n\n\n\n" << endl;
	ROS_INFO("JointSetup Start...");

	old_pantilt_data[0] = {2}; old_pantilt_data[1] = {2};

	//	fill_n(old_pantilt_data, 2, 0.01);

	old_arms_data[0] = {10}; old_arms_data[1] = {10}; old_arms_data[2] = {10};
	old_arms_data[3] = {10}; old_arms_data[4] = {10}; old_arms_data[5] = {10};

	//	fill_n(old_arms_data, 6, 0.01);

	pantilt_data[0] = {0.01}; pantilt_data[1] = {0.01};

	//	fill_n(pantilt_data, 2, 0.01);

	arms_data[0] = {0.01}; arms_data[1] = {0.01}; arms_data[2] = {0.01};
	arms_data[3] = {0.01}; arms_data[4] = {0.01}; arms_data[5] = {0.01};

	//	fill_n(arms_data, 6, 0.01);

	speed[0] = {0.00}; speed[1] = {0.00}; speed[2] = {0.00}; speed[3] = {0.00};
	speed[4] = {0.00}; speed[5] = {0.00}; speed[6] = {0.00}; speed[7] = {0.00};

	//	fill_n(speed, 8, 0.01);

	max_speed[0] = {1000}; max_speed[1] = {1000}; max_speed[2] = {1000}; max_speed[3] = {1000};
	max_speed[4] = {1000}; max_speed[5] = {1000}; max_speed[6] = {1000}; max_speed[7] = {1000};

	old_xtion_data = 0.01;
	xtion_data = 0.01;

	xtion_speed = 0.01;
	xtion_max_speed = 1000;

	isCall = false;

	_armServerProxy2 = new CArmERobot();
	_pantiltServerProxy2 = new CPanTiltERobot();

	__head_pan_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/head_pan/command",1000, &JointSetup::callbackHeadPan, this);
	__head_tilt_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/head_tilt/command",1000, &JointSetup::callbackHeadTilt, this);
	__xtion_tilt_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/xtion_tilt/command",1000, &JointSetup::callbackXtionTilt, this);
	__right_shoulder_pitch_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_shoulder_pitch/command",1000, &JointSetup::callbackRightShoulderPitch, this);
	__right_shoulder_roll_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_shoulder_roll/command",1000, &JointSetup::callbackRightShoulderRoll, this);
	__right_elbow_roll_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_elbow_roll/command",1000, &JointSetup::callbackRightElbowRoll, this);
	__left_shoulder_pitch_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_shoulder_pitch/command",1000, &JointSetup::callbackLeftShoulderPitch, this);
	__left_shoulder_roll_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_shoulder_roll/command",1000, &JointSetup::callbackLeftShoulderRoll, this);
	__left_elbow_roll_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_elbow_roll/command",1000, &JointSetup::callbackLeftElbowRoll, this);

	__head_pan_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/head_pan_max_speed/command",1000, &JointSetup::callbackHeadPanSpeed, this);
	__head_tilt_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/head_tilt_max_speed/command",1000, &JointSetup::callbackHeadTiltSpeed, this);
	__xtion_tilt_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/xtion_tilt_max_speed/command",1000, &JointSetup::callbackXtionTiltSpeed, this);
	__left_shoulder_pitch_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_shoulder_pitch_max_speed/command",1000, &JointSetup::callbackLeftShoulderPitchSpeed, this);
	__left_shoulder_roll_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_shoulder_roll_max_speed/command",1000, &JointSetup::callbackLeftShoulderRollSpeed, this);
	__left_elbow_roll_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/left_elbow_roll_max_speed/command",1000, &JointSetup::callbackLeftElbowRollSpeed, this);
	__right_shoulder_pitch_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_shoulder_pitch_max_speed/command",1000, &JointSetup::callbackRightShoulderPitchSpeed, this);
	__right_shoulder_roll_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_shoulder_roll_max_speed/command",1000, &JointSetup::callbackRightShoulderRollSpeed, this);
	__right_elbow_roll_speed_subscriber = this->__node_handle.subscribe<std_msgs::Float64>("/right_elbow_roll_max_speed/command",1000, &JointSetup::callbackRightElbowRollSpeed, this);

	__joint_state_pub = this->__node_handle.advertise<sensor_msgs::JointState>("/joint_states",1000);

	this->_bodyServiceServer = this->__node_handle.advertiseService("/body_start", &JointSetup::bodyDataServiceCallback, this);

	boost::thread( &JointSetup::moveJoint, this );
	boost::thread( &JointSetup::publishJointState, this );
}
 
JointSetup::~JointSetup() {
}
 
void JointSetup::callbackHeadPan(const std_msgs::Float64 msg){isCall = true; pantilt_data[0] = msg.data;}
void JointSetup::callbackHeadTilt(const std_msgs::Float64 msg){isCall = true; pantilt_data[1] = msg.data;}
void JointSetup::callbackXtionTilt(const std_msgs::Float64 msg){isCall = true; xtion_data = msg.data;}
void JointSetup::callbackRightShoulderPitch(const std_msgs::Float64 msg){isCall = true; arms_data[0] = msg.data;}
void JointSetup::callbackRightShoulderRoll(const std_msgs::Float64 msg){isCall = true; arms_data[1] = msg.data;}
void JointSetup::callbackRightElbowRoll(const std_msgs::Float64 msg){isCall = true; arms_data[2] = msg.data;}
void JointSetup::callbackLeftShoulderPitch(const std_msgs::Float64 msg){isCall = true; arms_data[3] = msg.data;}
void JointSetup::callbackLeftShoulderRoll(const std_msgs::Float64 msg){isCall = true; arms_data[4] = msg.data;}
void JointSetup::callbackLeftElbowRoll(const std_msgs::Float64 msg){isCall = true; arms_data[5] = msg.data;}

void JointSetup::callbackHeadPanSpeed(const std_msgs::Float64 msg){ max_speed[0] = msg.data;}
void JointSetup::callbackHeadTiltSpeed(const std_msgs::Float64 msg){ max_speed[1] = msg.data;}
void JointSetup::callbackXtionTiltSpeed(const std_msgs::Float64 msg){ xtion_max_speed = msg.data;}
void JointSetup::callbackLeftShoulderPitchSpeed(const std_msgs::Float64 msg){ max_speed[5] = msg.data;}
void JointSetup::callbackLeftShoulderRollSpeed(const std_msgs::Float64 msg){ max_speed[6] = msg.data;}
void JointSetup::callbackLeftElbowRollSpeed(const std_msgs::Float64 msg){ max_speed[7] = msg.data;}
void JointSetup::callbackRightShoulderPitchSpeed(const std_msgs::Float64 msg){ max_speed[2] = msg.data;}
void JointSetup::callbackRightShoulderRollSpeed(const std_msgs::Float64 msg){ max_speed[3] = msg.data;}
void JointSetup::callbackRightElbowRollSpeed(const std_msgs::Float64 msg){ max_speed[4] = msg.data;}

bool JointSetup::bodyDataServiceCallback(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response){

	double angles[6] = {0};
	int speeds[6] = {50, 50, 50, 50, 50, 50};

	_pantiltServerProxy2->moveToABSAll(0, 0, 0, 50, 50, 50);
	_armServerProxy2->moveBothArmToPositionWithAxisSpeed(angles, speeds);

	return RETURN_SUCCESS;
}

void JointSetup::moveJoint(){
 
	ros::Rate loop_rate(10);
 
	while( ros::ok() ){
 
		if (isCall){
			isCall = false;
 
			double curr_head_pan = pantilt_data[0]/M_PI*180;
			speed[0] = fabs((old_pantilt_data[0] - curr_head_pan)*10);
			if (speed[0] > max_speed[0]){
			speed[0] = max_speed[0];
			}
			old_pantilt_data[0] = curr_head_pan;

			double curr_head_tilt = pantilt_data[1]/M_PI*180;
			speed[1] = fabs((old_pantilt_data[1] - curr_head_tilt)*10);
			if (speed[1] > max_speed[1]){
			speed[1] = max_speed[1];
			}
			old_pantilt_data[1] = curr_head_tilt;

			double curr_xtion_data = xtion_data/M_PI*180;
			xtion_speed = fabs((old_xtion_data - curr_xtion_data)*10);
			if (xtion_speed > xtion_max_speed){
			xtion_speed = xtion_max_speed;
			}
			old_xtion_data = curr_xtion_data;

			double curr_right_shoulder_pitch = arms_data[0]/M_PI*180;
			speed[2] = fabs((old_arms_data[0] - curr_right_shoulder_pitch)*10);
			if (speed[2] > max_speed[2]){
			speed[2] = max_speed[2];
			}
			old_arms_data[0] = curr_right_shoulder_pitch;

			double curr_right_shoulder_roll = arms_data[1]/M_PI*180;
			speed[3] = fabs((old_arms_data[1] - curr_right_shoulder_roll)*10);
			if (speed[3] > max_speed[3]){
			speed[3] = max_speed[3];
			}
			old_arms_data[1] = curr_right_shoulder_roll;

			double curr_rigth_elbow_roll = arms_data[2]/M_PI*180;
			speed[4] = fabs((old_arms_data[2] - curr_rigth_elbow_roll)*10);
			if (speed[4] > max_speed[4]){
			speed[4] = max_speed[4];
			}
			old_arms_data[2] = curr_rigth_elbow_roll;

			double curr_left_shoulder_pitch = arms_data[3]/M_PI*180;
			speed[5] = fabs((old_arms_data[3] - curr_left_shoulder_pitch)*10);
			if (speed[5] > max_speed[5]){
			speed[5] = max_speed[5];
			}
			old_arms_data[3] = curr_left_shoulder_pitch;

			double curr_left_shoulder_roll = arms_data[4]/M_PI*180;
			speed[6] = fabs((old_arms_data[4] - curr_left_shoulder_roll)*10);
			if (speed[6] > max_speed[6]){
			speed[6] = max_speed[6];
			}
			old_arms_data[4] = curr_left_shoulder_roll;

			double curr_left_elbow_roll = arms_data[5]/M_PI*180;
			speed[7] = fabs((old_arms_data[5] - curr_left_elbow_roll)*10);
			if (speed[7] > max_speed[7]){
			speed[7] = max_speed[7];
			}
			old_arms_data[5] = curr_left_elbow_roll;

			double angles[6] = {curr_right_shoulder_pitch, curr_right_shoulder_roll, curr_rigth_elbow_roll,
			curr_left_shoulder_pitch, curr_left_shoulder_roll, curr_left_elbow_roll};
			int speeds[6] = {(int)speed[2], (int)speed[3], (int)speed[4], (int)speed[5], (int)speed[6], (int)speed[7]};

//			for (unsigned int i=0; i<6; i++) {
//				printf("%f ", angles[i]);
//			}
//			printf("\n");
//			for (unsigned int i=0; i<6; i++) {
//				printf("%d ", speeds[i]);
//			}
//			printf("\n");

			int degreeLeftRight = (int)curr_head_pan;
			int degreeUpDown = (int)curr_head_tilt;
			int speedLeftRight = (int)speed[0];
			int speedUpDown = (int)speed[1];

			int xtionAngle = (int)curr_xtion_data;
			int xtionSpeed = (int)xtion_speed;

			_pantiltServerProxy2->moveToABSAll(degreeLeftRight, degreeUpDown, xtionAngle, speedLeftRight, speedUpDown, xtionSpeed);
			_armServerProxy2->moveBothArmToPositionWithAxisSpeed(angles, speeds);
			}

		loop_rate.sleep();
	}
}
 
void JointSetup::publishJointState() {
 
	ros::Rate loop_rate(10);

	while( ros::ok() ){
		sensor_msgs::JointState msg;

		double* pos = new double[9];
		ERobotCore::getInstance()->getBodyPosition(pos);

		double head_pan = (double)pos[0]/180*M_PI;
		double head_tilt = (double)pos[1]/180*M_PI;
		double xtion_tilt = (double)pos[8]/180*M_PI;

		double left_shoulder_pitch = (double)pos[2]/180*M_PI;
		double left_shoulder_roll = (double)pos[3]/180*M_PI;
		double left_elbow_roll = (double)pos[4]/180*M_PI;

		double right_shoulder_pitch = (double)pos[5]/180*M_PI;
		double right_shoulder_roll = (double)pos[6]/180*M_PI;
		double right_elbow_roll = (double)pos[7]/180*M_PI;

		ros::Time now2 = ros::Time::now();

		msg.name = {"head_pan","head_tilt", "xtion_tilt",
					"left_shoulder_pitch", "left_shoulder_roll", "left_elbow_roll",
					"right_shoulder_pitch", "right_shoulder_roll", "right_elbow_roll"};

		msg.position = {-head_pan, head_tilt, xtion_tilt,
						right_shoulder_pitch, right_shoulder_roll, right_elbow_roll,
						left_shoulder_pitch, left_shoulder_roll, left_elbow_roll };

		msg.header.stamp = now2;
		this->__joint_state_pub.publish(msg);

		delete[] pos;

		loop_rate.sleep();
	}
}
 
double JointSetup::getRadian(double degree) {
	return ((degree * M_PI) / 180);
}