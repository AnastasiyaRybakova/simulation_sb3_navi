/*
 * CBatteryClient.cpp
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */


#include "CBatteryClient.h"

CBatteryClientProxy::CBatteryClientProxy()
{
}


CBatteryClientProxy::~CBatteryClientProxy()
{
}

CBattery* CBatteryClientProxy::newInstance() {
	//cout << "[CBatteryClientProxy] : Create Sole Instance(CBattery).." << endl;
	ROS_INFO("[CBatteryClientProxy] : Create Sole Instance(CBatteryROSStub)..");
	return new CBatteryROSStub();
}

CBatteryROSStub::CBatteryROSStub() {
	this->_batterySubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Battery_Msg>("/DeviceNode/Battery/data", 1000, &CBatteryROSStub::subscribeCallBack, this);

	ROS_INFO("[CBatteryROSStub] : check device node state.. ");
	while(this->_batterySubscriber.getNumPublishers() < 1) {
		ros::Duration(0.1).sleep();
	}
	this->_current_voltage = 100;
	ROS_INFO("   - check finished..(OK) \n");
}

CBatteryROSStub::~CBatteryROSStub()
{
}

CBatteryData CBatteryROSStub::readData( int index ) {

	unsigned int tempData[2] = {_current_voltage,0};
	CBatteryData returnData(
		0,
		0,
		tempData
		);
	return returnData;
}

bool CBatteryROSStub::isDocked(const int& id){
	return false;
}
bool CBatteryROSStub::doCharge(const int& id){
	return false;
}

void CBatteryROSStub::subscribeCallBack(const silbot3_msgs::Device_Battery_Msg::ConstPtr& message) {
	this->_current_voltage = message->voltage;
}
