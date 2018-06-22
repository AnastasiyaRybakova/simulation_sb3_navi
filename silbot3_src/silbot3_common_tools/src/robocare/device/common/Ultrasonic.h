#ifndef __ROSCIR_COMMON_ULTRASONIC_H__
#define __ROSCIR_COMMON_ULTRASONIC_H__

#include <robocare/device/data/UltrasonicData.h>

class CIRAPI CUltrasonic
{
public :
	virtual ~CUltrasonic() {}

	/** \brief
		read ultrasonic data
	\remarks
		after funtion call, it return ultrasonic data
	\return
		return CUltrasonicData
	\author
		cspark
	*/
	virtual CUltrasonicData readData( int index )=0;
	virtual void close() = 0;
};

#endif /* __ROSCIR_COMMON_ULTRASONIC_H__ */