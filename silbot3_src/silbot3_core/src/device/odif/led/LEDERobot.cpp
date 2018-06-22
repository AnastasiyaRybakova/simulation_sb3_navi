#include "LEDERobot.h"


CLightEmittingDiodeERobot::CLightEmittingDiodeERobot() {
	core = ERobotCore::getInstance();
}

CLightEmittingDiodeERobot::~CLightEmittingDiodeERobot() {
}

int CLightEmittingDiodeERobot::on( int id, int color ) {
	unsigned char blink = color << 24;;
	unsigned char red = color << 16;
	unsigned char green = color << 8;
	unsigned char blue = color << 0;

	unsigned char command = id;
	//int nRet = led->writeLedCommand(command, red, green, blue, blink);
	int nRet = core->setLEDValue(command, red, green, blue, blink);

	return nRet;
}

int CLightEmittingDiodeERobot::on(int id, unsigned char bright, unsigned char red, unsigned char green, unsigned char blue)
{
	//int nRet = led->writeLedCommand(id, red, green, blue, bright);
	int nRet = core->setLEDValue(id, red, green, blue, bright);
	return nRet;
}
