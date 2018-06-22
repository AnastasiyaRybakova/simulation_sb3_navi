#ifndef __TOUCH_SERVER_H__
#define __TOUCH_SERVER_H__

#include <iostream>
#include <string>

#include <ros/ros.h>
#include <reactive/erobot/ERobotCore.h>
#include <robocare/device/common/TouchButton.h>
#include <device/DeviceServerDefinition.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

class CTouchButtonERobot : public CTouchButton
{
private :
	ERobotCore* core;

public :
	CTouchButtonERobot();
	virtual ~CTouchButtonERobot();

	CTouchButtonData readData( int index );
	CHeadTouchButtonData readHeadData( int index );
	int detectingError(int touchID);
};
#endif /* __TOUCH_SERVER_H__ */
