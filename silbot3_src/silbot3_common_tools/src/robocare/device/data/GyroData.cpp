#include "GyroData.h"
#include <iostream>

using namespace std;
CGyroData::CGyroData( short rate, double angle )
{
	this->rate = rate;
	this->angle = angle;
}

CGyroData::~CGyroData()
{
}

short CGyroData::getRate()
{
	return this->rate;
}

double CGyroData::getAngle()
{
	return this->angle;
}

void CGyroData::print() 
{

	cout << "Gyro data << rate : " << rate << ", angle : " << angle << endl;
}
