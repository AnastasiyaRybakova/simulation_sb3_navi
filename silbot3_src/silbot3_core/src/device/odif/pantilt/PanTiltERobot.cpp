#include "PanTiltERobot.h"
#include <fstream>
//ofstream pantilt_fp;
CPanTiltERobot::CPanTiltERobot()
{
//	pantilt_fp.open("C:/opt/pantilt_write_byte.txt");
	core = ERobotCore::getInstance();
	isEmergency = false;
}

CPanTiltERobot::~CPanTiltERobot()
{
}

int CPanTiltERobot::on()
{
	return RETURN_SUCCESS;
}

int CPanTiltERobot::off()
{
	return RETURN_SUCCESS;
}

int CPanTiltERobot::stop()
{

	core->setPanTiltValue(EROBOT_COMMAND_PANTILT_STOP, 0,0,0,0);
	return RETURN_SUCCESS;
}

int CPanTiltERobot::moveToHome( int speed )
{
	if(isEmergency) return RETURN_SUCCESS;
	return core->setPanTiltValue(EROBOT_COMMAND_PANTILT_MOVE_TO_HOME, 0,0,0,0);
}

int CPanTiltERobot::moveToAbsolutePosition( double degreeLeftRight, double degreeUpDown, int speed ) {
	if(isEmergency) return RETURN_SUCCESS;
	if(degreeLeftRight > 65) {
		degreeLeftRight = 65;
	} else if (degreeLeftRight < -65) {
		degreeLeftRight = -65;
	}

	if(degreeUpDown > 40) {
		degreeUpDown = 40;
	} else if (degreeUpDown < -10) {
		degreeUpDown = -10;
	}

//	double s2 = (double)speed/6.4;
	speed = speed/378.0*100;

//	if(speed > 50) {
//		speed = 50;
//	}

	return core->setPanTiltValue(EROBOT_COMMAND_PANTILT_MOVE_TO_ABSOLUTE_POSITION, -1*degreeLeftRight, degreeUpDown, speed, speed);
}



int CPanTiltERobot::moveToAbsolutePositionWithSpeed(double degreeLeftRight, double degreeUpDown, int speedLeftRight, int speedUpDown)
{
//	cout << " input : " << degreeLeftRight << " , " << degreeUpDown << " , "<< speedLeftRight << " , "<< speedUpDown << endl;

	if(isEmergency) return RETURN_SUCCESS;
	if(degreeLeftRight > 65) {
		degreeLeftRight = 65;
	} else if (degreeLeftRight < -65) {
		degreeLeftRight = -65;
	}

	if(degreeUpDown > 40) {
		degreeUpDown = 40;
	} else if (degreeUpDown < -10) {
		degreeUpDown = -10;
	}

	speedLeftRight = speedLeftRight/330.0*100;
	speedUpDown = speedUpDown/378.0*100;

//	speedLeftRight = (int)(speedLeftRight*60/360/0.114/330*100 + 0.5);
//	speedUpDown = (int)(speedUpDown*60/360/0.114/378*100 + 0.5);

//	if(speedLeftRight < 5) speedLeftRight = 5;
//	if(speedUpDown < 5) speedUpDown = 5;
//	cout << " CPanTiltERobot::moveToAbsolutePositionWithSpeed : " << degreeLeftRight << " , " << degreeUpDown << " , "<< speedLeftRight << " , "<< speedUpDown << endl;
//	pantilt_fp << " moveToAbsolutePositionWithSpeed : " << degreeLeftRight << " , " << degreeUpDown << " , "<< speedLeftRight << " , "<< speedUpDown << endl;

	return core->setPanTiltValue(EROBOT_COMMAND_PANTILT_MOVE_TO_ABSOLUTE_POSITION, -1*degreeLeftRight, degreeUpDown, speedLeftRight, speedUpDown);
}

int CPanTiltERobot::moveToRelativePosition( double degreeLeftRight, double degreeUpDown, int speed )
{
	if(isEmergency) return RETURN_SUCCESS;
	cout << " Non implemented function: moveToRelativePosition " << endl;
	return RETURN_FAILED;
}

int CPanTiltERobot::move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 )
{

	printf("%d %d %d %d %d %d |", motorID1, motorSpeed1, motorPosition1, motorID2, motorSpeed2, motorPosition2);
	printf("\n");

	if(isEmergency) return RETURN_SUCCESS;
	cout << " Non implemented function: moveToRelativePosition " << endl;
	return RETURN_FAILED;
}

int CPanTiltERobot::moveMotor( int motorID, int motorSpeed, int motorPosition )
{
	if(isEmergency) return RETURN_SUCCESS;
	return RETURN_FAILED;
}


void CPanTiltERobot::getPosition(CPanTiltPosition& position)
{
	double angle[3];

	core->getPantiltPosition(angle);

	position.setDegreeLeftRight(angle[0]);
	position.setDegreeUpDown(angle[1]);
	position.setXtionTilt(angle[2]);
}

int CPanTiltERobot::setCompliance(int option)
{
	if(isEmergency) return RETURN_SUCCESS;
	return RETURN_FAILED;
}

int CPanTiltERobot::moveToABSAll(int panAngle, int tiltAngle, int xtionAngle, int panSpeed, int tiltSpeed, int xtionSpeed) {
	if(isEmergency) return RETURN_SUCCESS;
	if(panAngle > 65) {
		panAngle = 65;
	} else if (panAngle < -65) {
		panAngle = -65;
	}

	if(tiltAngle > 40) {
		tiltAngle = 40;
	} else if (tiltAngle < -10) {
		tiltAngle = -10;
	}

	if(xtionAngle > 45) {
		xtionAngle = 45;
	} else if (xtionAngle < -45) {
		xtionAngle = -45;
	}

	/**
	 * ������ ��ĥ ��
	 *
	 */

	double sp = (double)panSpeed/330.0*100;
	panSpeed = (int)sp;

	double st = (double)tiltSpeed/378.0*100;
	tiltSpeed = (int)st;

	double sx = (double)xtionSpeed/5.68;
	xtionSpeed = (int)sx;

//	if(panSpeed > 50) {
//		panSpeed = 50;
//	}
//	if(tiltSpeed > 50) {
//		tiltSpeed = 50;
//	}
//	if(xtionSpeed > 50) {
//		xtionSpeed = 50;
//	}

	ros::NodeHandle nh("~");
	int iXtionOffset = 0;
	nh.param<int>("core/pantitl/xion_offset", iXtionOffset, 0);

	if(iXtionOffset == -1){
		iXtionOffset = 0;
	}

	iXtionOffset = xtionAngle+ iXtionOffset;

	return core->setXtionPanTiltValue(EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL, -1*panAngle, tiltAngle, iXtionOffset, panSpeed, tiltSpeed, xtionSpeed);

}

int CPanTiltERobot::moveXtionTilt(int xtionAngle, int xtionSpeed) {

	double sx = (double)xtionSpeed/5.68;
	xtionSpeed = (int)sx;

	if(xtionSpeed > 50) {
		xtionSpeed = 50;
	}

	return core->setXtionPanTiltValue(EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL, 0, 0, xtionAngle, 0, 0, xtionSpeed);

}

int CPanTiltERobot::getXtionTiltAngle() {

	double angle[3];

	core->getPantiltPosition(angle);

	return angle[2];
}

void CPanTiltERobot::emergencyStop() {
	isEmergency = true;
}


void CPanTiltERobot::emergencyRelease() {
	isEmergency = false;
}

void CPanTiltERobot::detectingError(short errorCode[3]) {
	core->errorDetectingPantilt(errorCode);
}

void CPanTiltERobot::setHomePosition() {
	core->setXtionPanTiltValue(EROBOT_COMMAND_PANTILT_SET_HOME_POSITION, 0x07, 0, 0, 0, 0, 0);
}
