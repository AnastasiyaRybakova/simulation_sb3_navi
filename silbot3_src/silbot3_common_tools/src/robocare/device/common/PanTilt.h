#ifndef __ROSCIR_COMMON_PANTILT_H__
#define __ROSCIR_COMMON_PANTILT_H__

#include <robocare/device/data/PanTiltPosition.h>

#ifndef DEFINE_SCHEMA_CPanTilt
#define DEFINE_SCHEMA_CPanTilt
struct PANTILT_DATA_TYPE
{
	double degreeLeftRight;
	double degreeUpDown;
	int encoderLeft;
	int encoderRight;
};
#endif /* DEFINE_SCHEMA_CPanTilt */ 


class CIRAPI CPanTilt {
public:
	virtual ~CPanTilt() {}

public:

	virtual int on() = 0;
	virtual int off() = 0;
	virtual int stop() = 0;

	virtual int moveToAbsolutePositionWithSpeed( double degreeLeftRight, double degreeUpDown, int speedLeftRight, int speedUpDown) = 0;
	virtual int moveToRelativePosition( double degreeLeftRight, double degreeUpDown, int speed ) = 0;
	virtual int move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 )=0;
	virtual int moveMotor( int motorID, int motorSpeed, int motorPosition )=0;
	virtual void getPosition(CPanTiltPosition& position)=0;
	virtual int setCompliance(int option)=0;
	virtual int getXtionTiltAngle()=0;

// silbot3 using api

	virtual int moveToHome( int speed ) = 0;
	virtual int moveToAbsolutePosition( double degreeLeftRight, double degreeUpDown, int speed ) = 0;
	virtual int moveToABSAll(int panAngle, int tiltAngle, int xtionAngle, int panSpeed, int tiltSpeed, int xtionSpeed)=0;
	virtual int moveXtionTilt(int xtionAngle, int xtionSpeed)=0;
	virtual void emergencyStop() = 0;
	virtual void emergencyRelease() = 0;
	virtual void detectingError(short errorCode[3])=0;

};

#endif /* __ROSCIR_COMMON_PANTILT_H__ */
