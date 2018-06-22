#ifndef __ROSCIR_COMMON_ARM_H__
#define __ROSCIR_COMMON_ARM_H__

#include <robocare/device/data/ArmData.h>

class CIRAPI CArm
{
public:
	virtual ~CArm(void) {}

	virtual int on( int armID )=0;
	virtual int off( int armID )=0;
	virtual int setPeriodic( int armID, int milisecondTime )=0;

	virtual int moveToPositionAllWithPeriodic( int armID, double Angle, int stateType )=0;
	virtual CArmData waitRunOnPeriodic( float index )=0;
	virtual int stop()=0;

    virtual void getArmPosition(int armId, double angle[6]) = 0;
    virtual void setArmHomePosition() = 0;

// using api


	virtual int moveToHome( int armID )=0;
	virtual int moveToPositionAll( int armID, double* Angle, double Speed )=0;
	virtual int moveToPositionWithAxisSpeed( int armID, double* Angle, double* Speed )=0;
    virtual void moveBothArmToPositionAll(double Angle[6], int speed) = 0;
    virtual void moveBothArmToPositionWithAxisSpeed(double Angle[6], int speed[6]) = 0;

    virtual void emergencyStop() = 0;
    virtual void emergencyRelease() = 0;
    virtual void detectingError(short errorCode[6]) = 0;

};

#endif /* __ROSCIR_COMMON_ARM_H__ */
