#ifndef __ROSCIR_COMMON_WHEEL_H__
#define __ROSCIR_COMMON_WHEEL_H__

#include <robocare/device/CirApi.h>
#include <geometry_msgs/Twist.h>

class CIRAPI CWheel
{
protected :
	// (-800~800 mm/sec)



public :
	virtual ~CWheel() {}

	virtual int on()=0;
	virtual int off()=0;
	virtual int stop()=0;

	virtual int moveByWheelVelocity( double leftWheelVelocity, double rightWheelVelocity )=0;
	virtual int moveToDeltaX( int deltaX, double velocity )=0;
	virtual int moveToDeltaTheta( int deltaTheta, int radius, double velocity )=0;

	virtual int moveByWheelVelocity( double wheel1Rpm, double wheel2Rpm, double wheel3Rpm)=0;
	virtual int reset()=0;

	virtual int moveByTwistMsg(const geometry_msgs::Twist& twistMsg) = 0;

// silbot3 using apis

	virtual int moveByTRVelocity( double translationalVelocity, double rotationalVelocity )=0;
	virtual int moveToDeltaXYTheta( int deltaX, int deltaY, int deltaTheta, double velocity )=0;
	virtual int waitmoveToDeltaFinished()=0;
	virtual int moveByVelocityXYT( double velX, double velY, double velTheta)=0;

	virtual int emergencyStop() = 0;
	virtual int emergencyRelease() = 0;
	virtual int emergencyMoveByVelocityXYT( double velX, double velY, double velTheta)=0;
	virtual int selfDiagnosis(short errorCode[17])=0;

	virtual int startAutoCharge()=0;
	virtual int stopAutoCharge()=0;
};

#endif /* __ROSCIR_COMMON_WHEEL_H__ */
