#include "WheelEncoderHandler.h"
#include <math.h>
#include <ros/ros.h>
#define PI 3.14159265

using namespace cir::devicenode;

WheelEncoderHandler::WheelEncoderHandler() {
	_PreviousLeftEncoderCounts = 0;
	_PreviousRightEncoderCounts = 0;
	DistancePerCount = (3.14159265 * 0.1524) / 64000;

	_wheelEncoderServerProxy = CWheelEncoderERobot::getInstance();
	this->_WHEEL_ENCODER_THREAD_FLAG = false;
	initHandler();
	this->_this_thread = boost::thread(boost::bind(&WheelEncoderHandler::wheelEncoderPublishThread, this));
}

WheelEncoderHandler::~WheelEncoderHandler() {
	this->_WHEEL_ENCODER_THREAD_FLAG = false;
	this->_this_thread.join();
	ROS_DEBUG("[WheelEncoderHandler] closed... ");
}

void WheelEncoderHandler::initHandler() {
	//this->_wheelEncoderServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/WheelEncoderService", &WheelEncoderHandler::wheelEncoderServiceCallback, this);
	//ROS_INFO("WheelEncoder handler created - /DeviceNode/WheelEncoderService");
	this->_wheelEncoderPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_WheelEncoder_Msg>("/DeviceNode/WheelEncoder/data", 1000);
	this->_odom_pub = this->_nodeHandle.advertise<nav_msgs::Odometry>("odom", 50);
	ROS_INFO("WheelEncoderHandler handler created - /DeviceNode/WheelEncoder/data");
}

bool WheelEncoderHandler::wheelEncoderServiceCallback(silbot3_msgs::Device_WheelEncoder_Srv::Request &request, silbot3_msgs::Device_WheelEncoder_Srv::Response &response) {

	string command = request.command;

	if(command == COMMAND_WHEEL_ENCODER_READ_DATA) {
		CWheelEncoderData returnData = _wheelEncoderServerProxy->readData(1);
		response.msg.x = returnData.getX();
		response.msg.y = returnData.getY();
		response.msg.t = returnData.getTheta();

		return true;
	} else {
		cout << "[WheelEncoderHandler] unrecognized command. " << endl;
	}
	return false;
}

void WheelEncoderHandler::wheelEncoderPublishThread() {
	this->_WHEEL_ENCODER_THREAD_FLAG = true;
	ros::Rate loop_rate(10);
	tf::TransformBroadcaster odom_broadcaster;

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	last_time = current_time;

	CWheelEncoderData org_data = _wheelEncoderServerProxy->readData(1);
	double org_x = org_data.getX()/100;
	double org_y = org_data.getY()/100;

	double pose_x = 0.0f;
	double pose_y = 0.0f;

	while( ros::ok() && this->_WHEEL_ENCODER_THREAD_FLAG ) {

		silbot3_msgs::Device_WheelEncoder_Msg message;
		CWheelEncoderData data = _wheelEncoderServerProxy->readData(1);
		message.x = data.getX();
		message.y = data.getY();
		message.t = data.getTheta();


		this->_wheelEncoderPublisher.publish(message);
		ros::spinOnce();
		loop_rate.sleep();
	}
}
