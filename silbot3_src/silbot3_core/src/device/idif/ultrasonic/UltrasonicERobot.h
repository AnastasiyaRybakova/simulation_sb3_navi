#ifndef __ULTRASONIC_SERVER_H__
#define __ULTRASONIC_SERVER_H__

#include <ros/ros.h>
#include <iostream>
#include <robocare/device/common/Ultrasonic.h>
#include <reactive/erobot/ERobotCore.h>
#include <device/DeviceServerDefinition.h>

using namespace std;
using namespace reactive::erobot::core;

class CUltrasonicERobot : public CUltrasonic
{
private:
	CUltrasonicERobot();
	//ERobotSocketReceiver* sonic;
	ERobotCore* core;

private:
	int ultrasonicBumperRange;

public :
	~CUltrasonicERobot();
	static CUltrasonic* getInstance();

	CUltrasonicData readData(int index);
	void close();
};

#endif /* __ULTRASONIC_SERVER_H__ */
