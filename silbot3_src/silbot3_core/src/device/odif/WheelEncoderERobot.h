ifndef __WHEELENCODER_SERVER_H__
#define __WHEELENCODER_SERVER_H__

#include <ros/ros.h>
#include <iostream>
#include <robocare/device/common/WheelEncoder.h>
#include <robocare/device/data/WheelEncoderData.h>
#include <device/DeviceServerDefinition.h>
#include <reactive/erobot/ERobotCore.h>

using namespace std;
using namespace reactive::erobot::core;

class CWheelEncoderERobot : public CWheelEncoder
{
private:
	ERobotCore* core;

public :
	CWheelEncoderERobot();
    virtual ~CWheelEncoderERobot();
	CWheelEncoderData readData( int index);
};

#endif /* __WHEELENCODER_SERVER_H__ */
