#include "WheelEncoderData.h"
#include <iostream>

using namespace std;

CWheelEncoderData::CWheelEncoderData()
{
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;

	this->x = 0.;
	this->y = 0.;
	this->theta = 0.;
	
	this->numberOfData = NUMBER_OF_DATA;

}

CWheelEncoderData::CWheelEncoderData(long* data, int numberOfData) {
	this->numberOfData = numberOfData;
	for ( int inx = 0; inx < this->numberOfData; inx++ )
	{
		this->data[inx] = data[inx];
	}
}

CWheelEncoderData::CWheelEncoderData(long* data, const double& x, const double& y, const double& theta, int numberOfData) {
	this->numberOfData = numberOfData;
	for ( int inx = 0; inx < this->numberOfData; inx++ )
	{
		this->data[inx] = data[inx];
	}

	this->x = x;
	this->y = y;
	this->theta = theta;
}

const double& CWheelEncoderData::getX() const {
	return this->x;
}

const double& CWheelEncoderData::getY() const {
	return this->y;
}

const double& CWheelEncoderData::getTheta() const {
	return this->theta;
}

CWheelEncoderData::~CWheelEncoderData()
{
}

long* CWheelEncoderData::getData()
{
    return this->data;
}

const int& CWheelEncoderData::getNumberOfData() const {
	return this->numberOfData;
}

void CWheelEncoderData::print() {
	cout<<"============== encoder data =============="<<endl;
	cout<<"[";
	for(int i = 0; i < numberOfData; i++) {
		cout<<getData()[i]<<", ";
	}
	cout<<"]";
	cout<<endl;
	cout<<endl<<"=============== position data ==========="<<endl;
	cout<<"["<<getX()<<","<<getY()<<","<<getTheta()<<"]"<<endl;
	cout<<endl;
}
