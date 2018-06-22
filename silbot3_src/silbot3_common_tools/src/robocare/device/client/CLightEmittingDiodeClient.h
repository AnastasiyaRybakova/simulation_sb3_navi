#ifndef __ROSCIR_CLIENT_LED_H__
#define __ROSCIR_CLIENT_LED_H__

#include <robocare/device/RosCIRDefinitions.h>
#include <robocare/device/common/LightEmittingDiode.h>
#include <silbot3_msgs/Device_LED_Msg.h>

#include <iostream>
#include <string>

#include <ros/ros.h>

using namespace std;

class CIRAPI CLightEmittingDiodeClientProxy {
    private :
        CLightEmittingDiodeClientProxy();

    public :
        virtual ~CLightEmittingDiodeClientProxy();
        static CLightEmittingDiode* newInstance();
};


class CLightEmittingDiodeROSStub : public CLightEmittingDiode {
private :
	ros::NodeHandle			_nodeHandle;
	ros::Publisher			_ledPublisher;
public :
	CLightEmittingDiodeROSStub();
	virtual ~CLightEmittingDiodeROSStub();

	int on( int id, int color );
	int on(int id, unsigned char bright, unsigned char red, unsigned char green, unsigned char blue);
};
#endif /* __ROSCIR_CLIENT_LED_H__ */
