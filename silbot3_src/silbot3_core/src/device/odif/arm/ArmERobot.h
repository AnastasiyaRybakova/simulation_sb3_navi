#ifndef __ARM_SERVER_H__
#define __ARM_SERVER_H__

#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <robocare/device/common/Arm.h>
#include <reactive/erobot/ERobotCore.h>
#include <device/DeviceServerDefinition.h>

using namespace reactive::erobot::core;

class CArmERobot : public CArm
{
private:
	ERobotCore* core;
	bool isEmergency;
public :
	CArmERobot();
	~CArmERobot();

	int on( int armID );
	int off( int armID );
	int setPeriodic( int armID, int milisecondTime );
	int moveToHome( int armID );
	int moveToPositionAll( int armID, double* Angle, double Speed );
	int moveToPositionWithAxisSpeed( int armID, double* Angle, double* Speed );
	int moveToPositionAllWithPeriodic( int armID, double Angle, int stateType );
	int stop();
	CArmData waitRunOnPeriodic( float index );
	void close();

	void moveBothArmToPositionAll(double Angle[6], int speed);
	void moveBothArmToPositionWithAxisSpeed(double Angle[6], int speed[6]);
	void getArmPosition(int armId, double angle[6]);
	void setArmHomePosition();
	void emergencyStop();
	void emergencyRelease();
	void detectingError(short errorCode[6]);
};

#endif /* __ARM_SERVER_H__ */
