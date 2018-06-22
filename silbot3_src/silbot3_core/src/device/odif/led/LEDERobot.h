#ifndef __LED_SERVER_H__
#define __LED_SERVER_H__

#include <ros/ros.h>
#include <robocare/device/common/LightEmittingDiode.h>
#include <device/DeviceServerDefinition.h>
#include <reactive/erobot/ERobotCore.h>

using namespace std;
using namespace reactive::erobot::core;

class CLightEmittingDiodeERobot : public CLightEmittingDiode
{
private:

	ERobotCore* core;

public:
	CLightEmittingDiodeERobot();
	virtual ~CLightEmittingDiodeERobot();
public:
	int on( int id, int color );
	int on(int id, unsigned char bright, unsigned char red, unsigned char green, unsigned char blue);
};

#endif /* __LED_SERVER_H__ */
