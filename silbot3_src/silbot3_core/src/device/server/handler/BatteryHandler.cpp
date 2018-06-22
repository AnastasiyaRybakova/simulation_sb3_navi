/*
 * BatteryHandler.cpp
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */

#include "BatteryHandler.h"

using namespace cir::devicenode;

BatteryHandler::BatteryHandler() {
	_batteryServerProxy = new CBatteryERobot();
	this->_BATTERY_THREAD_FLAG = false;
	initHandler();
	this->_this_thread = boost::thread(boost::bind(&BatteryHandler::batteryPublishThread, this));
}

BatteryHandler::~BatteryHandler() {
	this->_BATTERY_THREAD_FLAG = false;
	this->_this_thread.join();
	ROS_DEBUG("[BatteryHandler] closed... ");
}

void BatteryHandler::initHandler() {
	this->_batteryPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Battery_Msg>("/DeviceNode/Battery/data", 1000);
	ROS_INFO("Battery handler created - /DeviceNode/Battery/data");
}

void BatteryHandler::batteryPublishThread() {
	this->_BATTERY_THREAD_FLAG = true;
	ros::Rate loop_rate(10);

	while( ros::ok() && this->_BATTERY_THREAD_FLAG ) {

		silbot3_msgs::Device_Battery_Msg message;

		CBatteryData data = _batteryServerProxy->readData(1);

		message.voltage = data.getData()[0];

		//cout << "message.voltage : " << message.voltage << endl;


		this->_batteryPublisher.publish(message);

		ros::spinOnce();
		loop_rate.sleep();
	}
}
