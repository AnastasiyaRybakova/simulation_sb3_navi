#ifndef __ROSCIR_DATA_PANTILT_H__
#define __ROSCIR_DATA_PANTILT_H__

#include <ostream>
#include <robocare/device/CirApi.h>

using namespace std;

class CIRAPI CPanTiltPosition
{
private :
	double degreeLeftRight;
	double degreeUpDown;
	int encoderLeft;
	int encoderRight;
	int xtionTilt;

public :
	CPanTiltPosition();
	~CPanTiltPosition();
	friend ostream& operator<<( ostream& os, const CPanTiltPosition& position );

	double getDegreeLeftRight();
	double getDegreeUpDown();
	int getEncoderLeft();
	int getEncoderRight();
	int getXtionTilt();

	void setDegreeLeftRight( double degreeLeftRight );
	void setDegreeUpDown( double degreeUpDown );
	void setEncoderLeft( int encoderLeft );
	void setEncoderRight( int encoderRight );
	void setXtionTilt(int tiltAngle);
};

#endif /* __ROSCIR_DATA_PANTILT_H__ */
