#include "ArmERobot.h"
#include <fstream>

CArmERobot::CArmERobot(void)
{
	core = ERobotCore::getInstance();
	isEmergency = false;
}

CArmERobot::~CArmERobot(void)
{
}

int CArmERobot::on( int armID )
{
	ROS_DEBUG("[CArmERobot] on");
	return 0;
}

int CArmERobot::off( int armID )
{
	ROS_DEBUG("[CArmERobot] off");
	return 0;
}

int CArmERobot::setPeriodic( int armID, int milisecondTime )
{
	ROS_DEBUG("[CArmERobot] setPeriodic");
	return 0;
}

int CArmERobot::moveToHome( int armID )
{
	if(isEmergency) {
		return 0;
	}
	ROS_DEBUG("[CArmERobot] moveToHome %d ",armID);
	short angle[6];
	short speed[6];
	memset(angle, 0x00, sizeof(angle));
	memset(speed, 0x00, sizeof(speed));

	return core->setArmValue(EROBOT_COMMAND_ARM_MOVE_TO_HOME,angle,speed);
}

/**
  * armID
  * 1 : right arm
  * 2 : left arm
  */
int CArmERobot::moveToPositionAll( int armID, double* Angle, double Speed )
{
	// cout << "moveToPositionAll..." << endl;

	if(isEmergency) {
		return 0;
	}
	ROS_DEBUG("[CArmERobot] moveToPositionAll");
	short angles[6];
	short speeds[6];

//	speeds[0] = (short)Speed/6.4;
//	speeds[1] = (short)Speed/6.4;
//	speeds[2] = (short)Speed/6.2;
//	speeds[3] = (short)Speed/6.4;
//	speeds[4] = (short)Speed/6.4;
//	speeds[5] = (short)Speed/6.2;

	speeds[0] = (short)Speed/3.78;
	speeds[1] = (short)Speed/3.78;
	speeds[2] = (short)Speed/3.3;
	speeds[3] = (short)Speed/3.78;
	speeds[4] = (short)Speed/3.78;
	speeds[5] = (short)Speed/3.3;

//	if(Speed > 20) {
//		Speed = 20;
//	}

	/**
	 * ������ ��ĥ ��
	 *
	 */
/*

	speeds[0] = Speed*60/360/0.114/190*100 + 0.5;
	speeds[1] = Speed*60/360/0.114/190*100 + 0.5;
	speeds[2] = Speed*60/360/0.111/344*100 + 0.5;
	speeds[3] = Speed*60/360/0.114/190*100 + 0.5;
	speeds[4] = Speed*60/360/0.114/190*100 + 0.5;
	speeds[5] = Speed*60/360/0.111/344*100 + 0.5;

	for(int k=0; k<6; k++){
		if(speeds[k] < 10) speeds[k] = 10;
	}
*/
	// Right Arm
	if ( 1 == armID ) {
		angles[0] = (short) Angle[0];
		angles[1] = (short) Angle[1];
		angles[2] = (short) Angle[2];
		speeds[3] = 0; speeds[4] = 0; speeds[5] = 0;
		if(angles[0] > 170) {
			angles[0] = 170;
		}
		if (angles[0] < -90) {
			angles[0] = -90;
		}

		if(angles[1] > 120) {
			angles[1] = 120;
		}
		if (angles[1] < -20) {
			angles[1] = -20;
		}



		if (angles[2] > 50) {
			angles[2] = 50;
			//cout << "Angle[2] MIN : " << ARM_2_MIN << endl;
		}

		if (angles[2] < -80) {
			angles[2] = -80;
		}
		//cout << "angles : " << angles[0] << " , " << angles[1] << " , " << angles[2] << " , " << angles[3] << " , " << angles[4] << " , " << angles[5] <<  endl;
		return core->setArmValue(EROBOT_COMMAND_ARM_MOVE_RIGHT_ARM,angles,speeds);
	} else if ( 0 == armID ) {
		angles[3] = (short) Angle[0];
		angles[4] = (short) Angle[1];
		angles[5] = (short) Angle[2];
		speeds[0] = 0; speeds[1] = 0; speeds[2] = 0;
		if(angles[3] > 170) {
			angles[3] = 170;
		}
		if (angles[3] < -90) {
			angles[3] = -90;
		}

		if(angles[4] > 120) {
			angles[4] = 120;
		}
		if (angles[4] < -20) {
			angles[4] = -20;
		}

		if (angles[5] > 50) {
			angles[5] = 50;
			//cout << "Angle[5] MIN : " << ARM_2_MIN << endl;
		}

		if (angles[5] < -80) {
			angles[5] = -80;
		}
		// cout << "angles : " << angles[0] << " , " << angles[1] << " , " << angles[2] << " , " << angles[3] << " , " << angles[4] << " , " << angles[5] <<  endl;
		return core->setArmValue(EROBOT_COMMAND_ARM_MOVE_LEFT_ARM,angles,speeds);
	} else {
		cout << " non-implemented function for armID. only armID 1, 2 is available" << endl;
		return RETURN_FAILED;
	}
}

int CArmERobot::moveToPositionAllWithPeriodic( int armID, double Angle, int stateType )
{
	cout << " non-implemented function, moveToPositionAllWithPeriodic. " << endl;
	return RETURN_FAILED;
}

int CArmERobot::stop()
{
	short angles[3];
	short speeds[3];

	core->setArmValue(EROBOT_COMMAND_ARM_STOP,angles,speeds);
	return RETURN_FAILED;
}

CArmData CArmERobot::waitRunOnPeriodic( float index )
{
	static CArmData data;
	cout << " non-implemented function, waitRunOnPeriodic. " << endl;
	return data;
}

void CArmERobot::moveBothArmToPositionAll( double Angle[6], int speed )
{
	if(isEmergency) {
		return;
	}
	ROS_DEBUG("[CArmERobot] moveBothArmToPositionAll");
	short allSpeed[6];

	double rAngle[3];
	rAngle[0] = Angle[0];
	rAngle[1] = Angle[1];
	rAngle[2] = Angle[2];

	double lAngle[3];
	lAngle[0] = Angle[3];
	lAngle[1] = Angle[4];
	lAngle[2] = Angle[5];
	double dSpeed = (double)speed;

	moveToPositionAll(0, lAngle, dSpeed);
	moveToPositionAll(1, rAngle, dSpeed);
}

int CArmERobot::moveToPositionWithAxisSpeed( int armID, double* Angle, double* Speed )
{
	if(isEmergency) {
		return 0;
	}
	ROS_DEBUG("[CArmERobot] moveToPositionWithAxisSpeed");
	short angles[6];
	short speeds[6];

	//	if(Speed > 20) {
	//		Speed = 20;
	//	}

	speeds[0] = Speed[0]/3.78;
	speeds[1] = Speed[1]/3.78;
	speeds[2] = Speed[2]/3.3;
	speeds[3] = Speed[0]/3.78;
	speeds[4] = Speed[1]/3.78;
	speeds[5] = Speed[2]/3.3;
/*
	speeds[0] = (short)(Speed[0]*60/360/0.114/482*100 + 0.5);
	speeds[1] = (short)(Speed[1]*60/360/0.114/482*100 + 0.5);
	speeds[2] = (short)(Speed[2]*60/360/0.111/873*100 + 0.5);
	speeds[3] = (short)(Speed[0]*60/360/0.114/482*100 + 0.5);
	speeds[4] = (short)(Speed[1]*60/360/0.114/482*100 + 0.5);
	speeds[5] = (short)(Speed[2]*60/360/0.111/873*100 + 0.5);

	for(int k=0; k<6; k++){
		if(speeds[k] < 5) speeds[k] = 5;
	}
	*/
	// Right Arm
//	cout << "angles : " << angles[0] << " , " << angles[1] << " , " << angles[2] << " , " << angles[3] << " , " << angles[4] << " , " << angles[5] <<  endl;

	if ( 1 == armID ) {

		angles[0] = (short) Angle[0];
		angles[1] = (short) Angle[1];
		angles[2] = (short) Angle[2];
		angles[3] = 0; angles[4] = 0; angles[5] = 0;
		speeds[3] = 0; speeds[4] = 0; speeds[5] = 0;
		if(angles[0] > 170) {
			angles[0] = 170;
		}
		if (angles[0] < -90) {
			angles[0] = -90;
		}

		if(angles[1] > 120) {
			angles[1] = 120;
		}
		if (angles[1] < -20) {
			angles[1] = -20;
		}



		if (angles[2] > 50) {
			angles[2] = 50;
			//cout << "Angle[2] MIN : " << ARM_2_MIN << endl;
		}

		if (angles[2] < -80) {
			angles[2] = -80;
		}

//		cout <<"angles : " << angles[0] << " , " << angles[1] << " , " << angles[2] << endl;
//		cout <<"speeds : " << speeds[0] << " , " << speeds[1] << " , " << speeds[2] << endl;
		return core->setArmValue(EROBOT_COMMAND_ARM_MOVE_RIGHT_ARM,angles,speeds);
	} else if ( 0 == armID ) {
		angles[3] = (short) Angle[0];
		angles[4] = (short) Angle[1];
		angles[5] = (short) Angle[2];
		angles[0] = 0; angles[1] = 0; angles[2] = 0;
		speeds[0] = 0; speeds[1] = 0; speeds[2] = 0;
		if(angles[3] > 170) {
			angles[3] = 170;
		}
		if (angles[3] < -90) {
			angles[3] = -90;
		}

		if(angles[4] > 120) {
			angles[4] = 120;
		}
		if (angles[4] < -20) {
			angles[4] = -20;
		}

		if (angles[5] > 50) {
			angles[5] = 50;
			//cout << "Angle[5] MIN : " << ARM_2_MIN << endl;
		}

		if (angles[5] < -80) {
			angles[5] = -80;
		}
		return core->setArmValue(EROBOT_COMMAND_ARM_MOVE_LEFT_ARM,angles,speeds);
	} else {
		cout << " non-implemented function for armID. only armID 1, 2 is available" << endl;
		return RETURN_FAILED;
	}

	return RETURN_FAILED;
}

void CArmERobot::moveBothArmToPositionWithAxisSpeed( double Angle[6], int speed[6] )
{

	if(isEmergency) {
		return;
	}
	ROS_DEBUG("[CArmERobot] moveBothArmToPositionWithAxisSpeed");
	short speeds[6];
	short angle[6];

	for (int i = 0 ; i < 6 ; i++ ){
		angle[i] = (short) Angle[i];
		speeds[i] = (short) speed[i];
//		printf("%d]] %d %f |", i, angle[i], Angle[i]);
	}
//	printf("\n");

	speeds[0] = speeds[0]/3.78;
	speeds[1] = speeds[1]/3.78;
	speeds[2] = speeds[2]/3.3;
	speeds[3] = speeds[3]/3.78;
	speeds[4] = speeds[4]/3.78;
	speeds[5] = speeds[5]/3.3;

//	for (int i = 0 ; i < 6 ; i++ ){
//		printf("%d]] %d %d |", i, angle[i], speed[i]);
//	}
//	printf("\n");

	core->setArmValue(EROBOT_COMMAND_ARM_MOVE_BOTH_ARM, angle, speeds);
}

void CArmERobot::getArmPosition( int armId, double angle[6] )
{
	ROS_DEBUG("[CArmERobot] getArmPosition");
	short angles[6] = {0,0,0,0,0,0};
	core->getArmPosition(angles);
	if (1 == armId) {
		angle[0] = (double)angles[0];
		angle[1] = (double)angles[1];
		angle[2] = (double)angles[2];
	} else if (0 == armId) {
		angle[0] = (double)angles[3];
		angle[1] = (double)angles[4];
		angle[2] = (double)angles[5];
	} else {
		angle[0] = (double)angles[0];
		angle[1] = (double)angles[1];
		angle[2] = (double)angles[2];
		angle[3] = (double)angles[3];
		angle[4] = (double)angles[4];
		angle[5] = (double)angles[5];
	}
}

void CArmERobot::setArmHomePosition() {
	ROS_DEBUG("[CArmERobot] setArmHomePosition");
	short angles[6] = {0x0F,0,0,0,0,0};
	short speeds[6] = {0,0,0,0,0,0};
//	cout << "CArmERobot::setArmHomePosition : " << EROBOT_COMMAND_ARM_SET_HOME_POSITION << endl;
	core->setArmHomePosition();
}

void CArmERobot::emergencyStop() {
	ROS_DEBUG("[CArmERobot] emergencyStop");
	isEmergency = true;
}
void CArmERobot::emergencyRelease() {
	ROS_DEBUG("[CArmERobot] emergencyRelease");
	isEmergency = false;
}
void CArmERobot::detectingError(short errorCode[6]) {
	core->errorDetectingArm(errorCode);
}
