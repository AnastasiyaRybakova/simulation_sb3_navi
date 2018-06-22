/*
 * CBatteryClient.h
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */

#ifndef __ROSCIR_CLIENT_BATTERY_H__
#define __ROSCIR_CLIENT_BATTERY_H__

#include <robocare/device/common/battery.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <iostream>

#include <ros/ros.h>
#include <silbot3_msgs/Device_Battery_Msg.h>

class CIRAPI CBatteryClientProxy {

private:
	CBatteryClientProxy();

public :
	virtual ~CBatteryClientProxy();
	static CBattery* newInstance();

};

class CBatteryROSStub : public CBattery {

private :
	ros::NodeHandle			_nodeHandle;
	ros::ServiceClient		_batteryClient;
	ros::Subscriber			_batterySubscriber;
	int						_current_voltage;

public :
	CBatteryROSStub();
    virtual ~CBatteryROSStub();

private :
	void subscribeCallBack(const silbot3_msgs::Device_Battery_Msg::ConstPtr& message);

public :
	CBatteryData readData( int index );
	bool isDocked(const int& id);
	bool doCharge(const int& id);
};


#endif /* __ROSCIR_CLIENT_BATTERY_H__ */
