#ifndef __ROSCIR_CLIENT_PANTILT_H__
#define __ROSCIR_CLIENT_PANTILT_H__

#include <robocare/device/common/PanTilt.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <iostream>

#include <ros/ros.h>
#include <silbot3_msgs/Device_Pantilt_Srv.h>
#include <silbot3_msgs/Device_Error_Srv.h>
#include <silbot3_msgs/Device_Data_Srv.h>
#include <silbot3_msgs/Device_ErobotPantilt_Msg.h>

class CIRAPI CPanTiltClientProxy
{
private :
	CPanTiltClientProxy();
	virtual ~CPanTiltClientProxy();
public:
	static CPanTilt* newInstance();
};

class CPanTiltROSStub : public CPanTilt {

private:
	ros::NodeHandle			_nodeHandle;
	ros::ServiceClient		_pantiltClient;
	ros::ServiceClient		_pantiltErrorClient;
	ros::ServiceClient		_pantiltPositionDataClient;
	ros::Publisher			_pantiltPublisher;
	void close();

public :
	CPanTiltROSStub();
	virtual ~CPanTiltROSStub();

	int on();
	int off();
	int stop();
	int moveToHome( int speed );
	int moveToAbsolutePosition( double degreeLeftRight, double degreeUpDown, int speed );
	int moveToAbsolutePositionWithSpeed( double degreeLeftRight, double degreeUpDown, int speedLeftRight, int speedUpDown);
	int moveToRelativePosition( double degreeLeftRight, double degreeUpDown, int speed );
	int move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 );
	int moveMotor( int motorID, int motorSpeed, int motorPosition );
	void getPosition(CPanTiltPosition& pos);
	int setCompliance(int option);
	int moveToABSAll(int panAngle, int tiltAngle, int xtionAngle, int panSpeed, int tiltSpeed, int xtionSpeed);
	int moveXtionTilt(int xtionAngle, int xtionSpeed);
	int getXtionTiltAngle();

	void emergencyStop();
	void emergencyRelease();
	void detectingError(short errorCode[3]);
};

#endif /* __ROSCIR_CLIENT_PANTILT_H__ */
