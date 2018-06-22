#ifndef __ROSCIR_CLIENT_WHEELENCODER_H__
#define __ROSCIR_CLIENT_WHEELENCODER_H__

#include <robocare/device/common/WheelEncoder.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <iostream>

#include <ros/ros.h>
#include <silbot3_msgs/Device_WheelEncoder_Srv.h>
#include <silbot3_msgs/Device_WheelEncoder_Msg.h>

class CIRAPI CWheelEncoderClientProxy {

private:
	CWheelEncoderClientProxy();

public :
	virtual ~CWheelEncoderClientProxy();
	static CWheelEncoder* newInstance();

};

class CWheelEncoderROSStub : public CWheelEncoder {

private :
	ros::NodeHandle			_nodeHandle;
	ros::ServiceClient		_wheelEncoderClient;
	ros::Subscriber			_wheelEncoderSubscriber;
	double					_this_x;
	double					_this_y;
	double					_this_t;

public :
	CWheelEncoderROSStub();
    virtual ~CWheelEncoderROSStub();

private :
	void subscribeCallBack(const silbot3_msgs::Device_WheelEncoder_Msg::ConstPtr& message);

public :
	CWheelEncoderData readData( int index );
};

#endif /* __ROSCIR_CLIENT_WHEELENCODER_H__ */
