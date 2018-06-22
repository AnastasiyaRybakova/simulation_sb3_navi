#ifndef __ROSCIR_COMMON_GYRO_H__
#define __ROSCIR_COMMON_GYRO_H__

#include <robocare/device/data/GyroData.h>

class CGyro
{
public:
	virtual ~CGyro(void) {}

public:

	/** \brief
		read gyro data
	\remarks
		after funtion call, it return gyro data
	\return
		GyroData
	\author
		cspark
	*/
	virtual CGyroData readData()=0;
};

#endif /* __ROSCIR_COMMON_GYRO_H__ */