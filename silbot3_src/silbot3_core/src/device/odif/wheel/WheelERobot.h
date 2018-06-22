#ifndef __WHEEL_SERVER_H__
#define __WHEEL_SERVER_H__

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <robocare/device/common/Wheel.h>
#include <device/DeviceServerDefinition.h>
#include <reactive/erobot/ERobotCore.h>

using namespace std;
using namespace reactive::erobot::core;

typedef struct wheelCommand WHEEL_COMMAND;

struct wheelCommand
{
	int    commandID;               // Command ID
	double translationalVelocity;   // translationalVelocity
	double rotationalVelocity;      // rotationalVelocity
};

class CWheelERobot : public CWheel
{
private:
	ERobotCore* core;
	bool isEmergency;
public :
	CWheelERobot();
	~CWheelERobot();

	int on();
	int off();
	int stop();
	int moveByTRVelocity( double translationalVelocity, double rotationalVelocity );
	int moveByWheelVelocity( double leftWheelVelocity, double rightWheelVelocity );
	int moveToDeltaX( int deltaX, double velocity );
	int moveToDeltaTheta( int deltaTheta, int radius, double velocity );
	int moveToDeltaXYTheta( int deltaX, int deltaY, int deltaTheta, double velocity );
	int waitmoveToDeltaFinished();
	int moveByVelocityXYT(double velX, double velY, double velTheta);
	int moveByWheelVelocity( double wheel1Rpm, double wheel2Rpm, double wheel3Rpm);
	int reset();
	int moveByTwistMsg(const geometry_msgs::Twist& twistMsg);
	int emergencyStop();
	int emergencyRelease();
	int emergencyMoveByVelocityXYT( double velX, double velY, double velTheta);
	int selfDiagnosis(short errorCode[9]);
	int startCharge();
	int selfDiagnosis();
	int startAutoCharge();
	int stopAutoCharge();
};

#endif /* __WHEEL_SERVER_H__ */
