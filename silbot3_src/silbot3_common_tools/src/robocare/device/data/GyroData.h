#ifndef __ROSCIR_DATA_GYRO_H__
#define __ROSCIR_DATA_GYRO_H__

#include <robocare/device/CirApi.h>

class CIRAPI CGyroData
{
private:
	short rate;
	double angle;

public:
	CGyroData(short rate, double angle);
	~CGyroData(void);

public:
	short getRate();
	double getAngle();
	void print();
};

#endif /* __ROSCIR_DATA_GYRO_H__ */