#include "GyroERobot.h"

CGyroERobot::CGyroERobot() {
	_core = new EGyroCore;
	// _comPort = CConfig::getInstance()->getParamValue("GYRO_COM_PORT");
}

CGyroERobot::~CGyroERobot() {
	if(_core) {
		delete _core;
	}
}

CGyro* CGyroERobot::getInstance() {
	static CGyroERobot thisInstance;
	return &thisInstance;
}

CGyroData CGyroERobot::readData() {
	if(this->_comPort == "COM-1") {
		return CGyroData(-1, -1);
	}
	return _core->getData();
}
