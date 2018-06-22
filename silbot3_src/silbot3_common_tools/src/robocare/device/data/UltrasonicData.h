#ifndef __ROSCIR_DATA_ULTRASONIC_H__
#define __ROSCIR_DATA_ULTRASONIC_H__

#include <robocare/device/CirApi.h>

class CIRAPI CUltrasonicData
{
public :
	static const int NUMBER_OF_DATA = 12;
	unsigned short data[ NUMBER_OF_DATA ];


public :
	CUltrasonicData();
	CUltrasonicData( int status, unsigned int time, unsigned short* data );
	~CUltrasonicData();

	unsigned short* getData();
};

#endif /* __ROSCIR_DATA_ULTRASONIC_H__ */