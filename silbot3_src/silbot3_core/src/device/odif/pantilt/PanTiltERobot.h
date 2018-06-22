#ifndef __PANTILT_SERVER_H__
#define __PANTILT_SERVER_H__

#include <ros/ros.h>
#include <robocare/device/common/PanTilt.h>
#include <reactive/erobot/ERobotCore.h>
#include <device/DeviceServerDefinition.h>

using namespace std;

using namespace reactive::erobot::core;

class CPanTiltERobot : public CPanTilt
{
private:
	ERobotCore* core;
	bool isEmergency;
public :
	CPanTiltERobot();
	virtual ~CPanTiltERobot();

	int on();
	int off();
	int stop();
	int moveToHome( int speed );
	int moveToAbsolutePosition( double degreeLeftRight, double degreeUpDown, int speed );
	int moveToAbsolutePositionWithSpeed( double degreeLeftRight, double degreeUpDown, int speedLeftRight, int speedUpDown);
	int moveToRelativePosition( double degreeLeftRight, double degreeUpDown, int speed );
	int move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 );
	int moveMotor( int motorID, int motorSpeed, int motorPosition );
	void getPosition(CPanTiltPosition& position);
	int setCompliance(int option);
	int moveToABSAll(int panAngle, int tiltAngle, int xtionAngle, int panSpeed, int tiltSpeed, int xtionSpeed);
	int moveXtionTilt(int xtionAngle, int xtionSpeed);
	int getXtionTiltAngle();
	void emergencyStop();
	void emergencyRelease();
	void detectingError(short errorCode[3]);
	void setHomePosition();
};

#endif /* __PANTILT_SERVER_H__ */
