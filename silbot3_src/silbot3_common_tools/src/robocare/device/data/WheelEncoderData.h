#ifndef __ROSCIR_DATA_WHEELENCODER_H__
#define __ROSCIR_DATA_WHEELENCODER_H__

#include <robocare/device/CirApi.h>

#define DEG_RAD 				(M_PI/180.0f)
#define RAD_DEG					(180.0f/M_PI)
#define to_radian(d)			(float)(((float)(d))*DEG_RAD)
#define to_degree(r)			(float)(((float)(r))*RAD_DEG)
#define M_PI 3.14159265358979323846

class CIRAPI CWheelEncoderData {
public:
	static const int NUMBER_OF_DATA = 3;
public:
	long data[NUMBER_OF_DATA];
	double x, y, theta;
	int numberOfData;
public:
	CWheelEncoderData();
	CWheelEncoderData(long* data, int numberOfData);
	CWheelEncoderData(long* data, const double& x, const double& y, const double& theta, int numberOfData);
	virtual ~CWheelEncoderData();
	long* getData();
	const int& getNumberOfData() const;

	const double& getX() const;
	const double& getY() const;
	const double& getTheta() const;

	void print();
};

#endif /* __ROSCIR_DATA_WHEELENCODER_H__ */