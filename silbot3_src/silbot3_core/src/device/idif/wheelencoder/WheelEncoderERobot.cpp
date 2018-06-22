#include "WheelEncoderERobot.h"
//#include <reactive/erobot/ERobotSocketReceiver.h>
#include <iostream>


using namespace reactive::erobot::core;

CWheelEncoderERobot::CWheelEncoderERobot()
{
	//wheel = ERobotSocketReceiver::getInstance();
	core = ERobotCore::getInstance();
	//cout << "WheelEncoderERobot..." << endl;
}

CWheelEncoderERobot::~CWheelEncoderERobot()
{
	//delete wheel;
}

CWheelEncoder* CWheelEncoderERobot::getInstance() {
	static CWheelEncoderERobot thisInstance;
	return &thisInstance;
}

CWheelEncoderData CWheelEncoderERobot::readData(int index)
{
	long a[3] = { 0 , 0, 0};
	long datas[6] = {0,0,0,0,0,0};
	int x=0, y=0, t=0;

	//wheel->getWheelPosition(&x, &y, &t);
	core->getWheelEncoderValue(a, &x, &y, &t);
	//erobot�� ���� ��Ű ���� x/y���� �ٸ�.
	CWheelEncoderData encoderData(a, y, -1 * x, t, 3);
	//cout << "WheelEncoderERobot.readData : " << a[0] << ", " << a[1] << ", " << a[2] <<", position: " << datas[3] << ", " << datas[4] << ", " << datas[5] << endl;

	return encoderData;
}
