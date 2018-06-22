#include "WheelERobot.h"

CWheelERobot::CWheelERobot(void)
{
	core = ERobotCore::getInstance();
	isEmergency = false;
}

CWheelERobot::~CWheelERobot(void)
{
}

int CWheelERobot::on()
{
	return RETURN_SUCCESS;
}

int CWheelERobot::off()
{
	return RETURN_SUCCESS;
}

int CWheelERobot::stop()
{
	ROS_DEBUG("[CWheelERobot] stop");
	return core->setWheelValue(EROBOT_COMMAND_WHEEL_EMERGENCY_STOP, 0, 0, 0, 0);
}

int CWheelERobot::moveByTRVelocity( double translationalVelocity, double rotationalVelocity )
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveByTRVelocity");
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_BY_TR_VELOCITY, (int)translationalVelocity/2, (int)rotationalVelocity/2 * -1, 0, 0 );
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::moveByWheelVelocity( double leftWheelVelocity, double rightWheelVelocity )
{

	cout << " CWheelERobot.moveByWheelVelocity(param2) is NOT implemented. " << endl;
	return RETURN_FAILED;
}

int CWheelERobot::moveToDeltaX( int deltaX, double velocity )
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveToDeltaX");
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT, 0, -1 * deltaX, 0, velocity);
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::moveToDeltaTheta( int deltaTheta, int radius, double velocity )
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveToDeltaTheta");
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT, 0, 0, deltaTheta, velocity);
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::moveToDeltaXYTheta( int deltaX, int deltaY, int deltaTheta, double velocity )
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveToDeltaXYTheta %d , %d, %d %f", deltaX, deltaY, deltaTheta, velocity);
//		cout << "CWheelERobot::moveToDeltaXYTheta : " << deltaX << " , " << deltaY << " , " << deltaTheta << " , " << velocity << endl;
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT, deltaY*-1, deltaX, deltaTheta, velocity);
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::waitmoveToDeltaFinished() {
	return RETURN_SUCCESS;
}

int CWheelERobot::moveByVelocityXYT( double velX, double velY, double velTheta )
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveByVelocityXYT %f , %f, %f", velX, velY, velTheta);
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT,  -1 *velY/2, velX/2, -1 *velTheta/2, 0);
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::moveByWheelVelocity(double wheel1Rpm, double wheel2Rpm, double wheel3Rpm)
{
	if(!isEmergency) {
		ROS_DEBUG("[CWheelERobot] moveByWheelVelocity");
		return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY, (int)wheel1Rpm, (int)wheel2Rpm, (int)wheel3Rpm, 0);
	} else {
		return RETURN_SUCCESS;
	}
}

int CWheelERobot::reset()
{
	return RETURN_SUCCESS;
}

int CWheelERobot::moveByTwistMsg(const geometry_msgs::Twist& twistMsg) {
	return RETURN_SUCCESS;
}

int CWheelERobot::emergencyStop()
{
	ROS_DEBUG("[CWheelERobot] emergencyStop");
	if(!isEmergency) {
		isEmergency = true;
		core->setWheelValue(EROBOT_COMMAND_WHEEL_EMERGENCY_STOP, 0, 0, 0, 0);
	}
	return RETURN_SUCCESS;
}

int CWheelERobot::emergencyRelease()
{
	ROS_DEBUG("[CWheelERobot] emergencyRelease");
	isEmergency = false;
	return RETURN_SUCCESS;
}
int CWheelERobot::emergencyMoveByVelocityXYT( double velX, double velY, double velTheta) {
	ROS_DEBUG("[CWheelERobot] moveByVelocityXYT %f , %f, %f", velX, velY, velTheta);
	return core->setWheelValue(EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT,  -1 *velY/2, velX/2, -1 *velTheta/2, 0);
}

int CWheelERobot::selfDiagnosis(short errorCode[9]) {
	core->selfDiagnosisWheel(errorCode);
	return RETURN_SUCCESS;
}


int CWheelERobot::startAutoCharge() {
	core->startAutoCharge();
	return RETURN_SUCCESS;
}

int CWheelERobot::stopAutoCharge() {
	core->stopAutoCharge();
	return RETURN_SUCCESS;
}
