#include "TouchButtonERobot.h"


CTouchButtonERobot::CTouchButtonERobot()
{
	core = ERobotCore::getInstance();
	cout << "ERobotTouchButton." << endl;
}

CTouchButtonERobot::~CTouchButtonERobot() {

}

CTouchButtonData CTouchButtonERobot::readData( int index ) {
	//unsigned short udata[CTouchButtonData::NUMBER_OF_DATA];
	//core->getTouchButtonValue(udata, CTouchButtonData::NUMBER_OF_DATA);
	//CTouchButtonData data(udata, CTouchButtonData::NUMBER_OF_DATA);

	unsigned short udata[6];
	core->getTouchButtonValue(udata, 6);

	CTouchButtonData data(udata, 6);

	return data;
}

CHeadTouchButtonData CTouchButtonERobot::readHeadData( int index ) {
	return CHeadTouchButtonData();
}

int CTouchButtonERobot::detectingError(int touchID) {
	return core->errorDetectingTouch(touchID);
}
