#ifndef __ROSCIR_COMMON_ROLLBEND_H__
#define __ROSCIR_COMMON_ROLLBEND_H__

#include <robocare/device/data/RollBendPosition.h>

#ifndef DEFINE_SCHEMA_CRollBend
#define DEFINE_SCHEMA_CRollBend

struct ROLLBEND_DATA_TYPE
{
	double degreeRoll;
	double degreeBend;
	int encoderLeft;
	int encoderRight;
};

#endif

class CIRAPI CRollBend {
public:
	virtual ~CRollBend() {}

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
	virtual int on() = 0;


	/** \brief
		destroy connection 
	\remarks
		try to disconnect device-server
	\return
		1:success  0:fail
	\author
		cspark
	*/
    virtual int off() = 0;


	/** \brief
		stop
	\remarks
		stop all motor
	\return
		1:success  0:fail
	\author
		cspark
	*/
    virtual int stop() = 0;


	/** \brief
		move to home
	\remarks
		move to home position with speed
	\return
		1:success  0:fail
	\author
		cspark
	*/
    virtual int moveToHome( int speed ) = 0;


	/** \brief
		move to AbsolutePosition
	\remarks
		move to position with AbsolutePosition
	\return
		1:success  0:fail
	\author
		cspark
	*/
    virtual int moveToAbsolutePosition( double degreeRoll, double degreeBend, int speed ) = 0;



    virtual int moveToAbsolutePositionWithSpeed( double degreeRoll, double degreeBend, int speedRoll, int speedBend ) = 0;


	/** \brief
		move to RelativePosition
	\remarks
		move to position with RelativePosition
	\return
		1:success  0:fail
	\author
		cspark
	*/
    virtual int moveToRelativePosition( double degreeRoll, double degreeBend, int speed ) = 0;
	
	
	/** \brief
		move2MotorSync
	\remarks
		move to position with motorID and speed.
	\return
		1:success  0:fail
	\author
		cspark
	*/
	virtual int move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 )=0;
    
	
	/** \brief
		moveMotor
	\remarks
		move motor with id and speed
	\return
		1:success  0:fail
	\author
		cspark
	*/
	virtual int moveMotor( int motorID, int motorSpeed, int motorPosition )=0;


	/** \brief
		read rollbend data
	\remarks
		after funtion call, it return rollbend data
	\return
		return CRollBendPosition
	\author
		cspark
	*/
    virtual void getPosition(CRollBendPosition& position)=0;
    
    virtual void detectingError(short errorCode[3])=0;

};

#endif /* __ROSCIR_COMMON_ROLLBEND_H__ */
