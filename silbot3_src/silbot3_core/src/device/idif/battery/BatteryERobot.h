#ifndef __BATTERY_SERVER_H__
#define __BATTERY_SERVER_H__

#include <ros/ros.h>
#include <iostream>
#include <boost/thread.hpp>
#include <robocare/device/common/battery.h>
#include <device/DeviceServerDefinition.h>
#include <reactive/erobot/ERobotCore.h>

class CBatteryERobot : public CBattery
{
private :
	ERobotCore* core;

public:
	CBatteryERobot(void);
	~CBatteryERobot(void);

	static CBattery* getInstance();

	CBatteryData readData( int index );
    bool isDocked(const int& id);
    bool doCharge(const int& id);
};

#endif /* __BATTERY_SERVER_H__ */
