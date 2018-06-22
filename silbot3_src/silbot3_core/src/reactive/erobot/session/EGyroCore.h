#ifndef __EROBOT_GYRO_CORE_H__
#define __EROBOT_GYRO_CORE_H__

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <ros/ros.h>

#include <boost/thread/thread.hpp>
#include <robocare/device/common/Gyro.h>

using namespace std;

namespace reactive {
namespace erobot {
namespace core {


class EGyroCore {
private:
//				ERobotSerialConnector _conn;
//				double _rateResult;
//				double _angleResult;
//				double _init_angle;
//				bool _INIT_FLAG;
//
	boost::thread _this_thread;


	short _rate;
	double _angle;

	bool fistFlag;

	double _angleOffset;

public:
	EGyroCore();
	virtual ~EGyroCore();

private:
	void init();
	void run();
	void ccr1050_getvalue();


public:
	bool findHeader(const int& fd);
	bool readValue(const int& fd);
	CGyroData getData();

};



}}}

#endif /* __EROBOT_GYRO_CORE_H__ */
