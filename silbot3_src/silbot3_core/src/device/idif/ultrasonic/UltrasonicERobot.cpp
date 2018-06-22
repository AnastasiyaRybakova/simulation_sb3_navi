#include "UltrasonicERobot.h"

CUltrasonicERobot::CUltrasonicERobot()
{
	core = ERobotCore::getInstance();
}


CUltrasonicERobot::~CUltrasonicERobot()
{
}

CUltrasonic* CUltrasonicERobot::getInstance( )
{
	static CUltrasonicERobot thisInstance;
	return &thisInstance;
}


CUltrasonicData CUltrasonicERobot::readData(int index)
{
	unsigned short result[CUltrasonicData::NUMBER_OF_DATA];

	memset(result, 0x00, CUltrasonicData::NUMBER_OF_DATA);
	core->getUltrasonicValue(result, CUltrasonicData::NUMBER_OF_DATA);

	CUltrasonicData ultrasonicData(0,0, result);
	return ultrasonicData;
}


void CUltrasonicERobot::close() {


}
