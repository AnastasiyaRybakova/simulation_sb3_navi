#include "BatteryERobot.h"

CBatteryERobot::CBatteryERobot(void)
{
	core = ERobotCore::getInstance();
}

CBatteryERobot::~CBatteryERobot(void)
{
}

CBattery* CBatteryERobot::getInstance( ) {
	static CBatteryERobot thisInstance;
	return &thisInstance;
}

CBatteryData CBatteryERobot::readData( int index ) {
	float exb = 0;
	float inb = 0;

	int value = 0;
	//rkedbp_battery_percent(&exb, &inb);
	core->getBatteryValue(&value);

	unsigned int data[CBatteryData::NUMBER_OF_DATA];
	data[0] = (unsigned int)value;

	CBatteryData battData(0, 0, data);
	return battData;
}

bool CBatteryERobot::isDocked(const int& id) {
	return false;
}

bool CBatteryERobot::doCharge(const int& id) {
	return false;
}
