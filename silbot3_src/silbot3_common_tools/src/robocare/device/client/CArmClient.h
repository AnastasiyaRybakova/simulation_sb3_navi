#ifndef __ROSCIR_CLIENT_ARM_H__
#define __ROSCIR_CLIENT_ARM_H__

#include <robocare/device/common/Arm.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <iostream>

#include <ros/ros.h>
#include <silbot3_msgs/Device_Arm_Srv.h>
#include <silbot3_msgs/Device_Error_Srv.h>
#include <silbot3_msgs/Device_Arm_Msg.h>

using namespace std;

class CIRAPI CArmClientProxy
{
private:
	CArmClientProxy(void);

public:
	virtual ~CArmClientProxy(void);

public:
	static CArm* newInstance();
};


class CArmROSStub : public CArm {

private:
	ros::NodeHandle			_nodeHandle;
	ros::ServiceClient		_armClient;
	ros::ServiceClient		_armErrorClient;
	ros::Publisher			_armPublisher;

public:
	CArmROSStub(void);
	virtual ~CArmROSStub(void);

public:

	/** \brief
		establishing connection
	\remarks
		try to connect device-server
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int on( int armID );



	/** \brief
		destroy connection
	\remarks
		try to disconnect device-server
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int off( int armID );


	/** \brief
		not support
	\remarks
		.
	\return
		-1
	\author
		cspark
	*/
	int setPeriodic( int armID, int milisecondTime );


	/** \brief
		Initialize position
	\remarks
		set arm motor to init position
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int moveToHome( int armID );


	/** \brief
		move arm
	\remarks
		move arm with Arm id(left or right), angle and speed
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int moveToPositionAll( int armID, double* Angle, double Speed );



	/** \brief
		move arm
	\remarks
		move arm with Arm id(left or right), angle and speed
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int moveToPositionWithAxisSpeed( int armID, double* Angle, double* Speed );


	/** \brief
		move arm
	\remarks
		move arm with Arm id(left or right), angle and speed
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int moveToPositionAllWithPeriodic( int armID, double Angle, int stateType );


	/** \brief
		stop arm
	\remarks
		stop arm
	\return
		1:success  0:fail
	\author
		cspark
	*/
	int stop();



	/** \brief
		not support
	\remarks
		return arm position data
	\return
		arm data
	\author
		cspark
	*/
	CArmData waitRunOnPeriodic( float index );



	//
	// for engkey
	//
	//

	/** \brief
		move arm (for engkey)
	\remarks
		move arm with angle and speed. angle array size is 6. move same speed
	\return
		void
	\author
		cspark
	*/
	void moveBothArmToPositionAll(double Angle[6], int speed);

	/** \brief
		move arm (for engkey)
	\remarks
		move arm with angle and speed. angle array size is 6. move different speed
	\return
		void
	\author
		cspark
	*/
	void moveBothArmToPositionWithAxisSpeed(double Angle[6], int speed[6]);


	/** \brief
		get arm position
	\remarks
		after funtion call, it return arm data
	\return
		void
	\author
		cspark
	*/
	void getArmPosition(int armId, double angle[6]);
	void setArmHomePosition();
	void emergencyStop();
	void emergencyRelease();
	void detectingError(short errorCode[6]);
};


#endif /* __ROSCIR_CLIENT_ARM_H__ */
