#ifndef __GYRO_SERVER_H__
#define __GYRO_SERVER_H__

#include <ros/ros.h>
#include <iostream>
#include <boost/thread.hpp>
#include <robocare/device/common/Gyro.h>
#include <device/DeviceServerDefinition.h>
#include <reactive/erobot/session/EGyroCore.h>

using namespace std;
using namespace reactive::erobot::core;

class CGyroERobot : public CGyro
{
private :
	CGyroERobot();
	EGyroCore* _core;
	string _comPort;

public :
	~CGyroERobot();
	static CGyro* getInstance();

public :
	CGyroData readData();
};

#endif /* __GYRO_SERVER_H__ */
