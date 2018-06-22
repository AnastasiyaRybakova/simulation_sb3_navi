#include "DeviceConnector.h"

using namespace expression::connector;

DeviceConnector::DeviceConnector() {
	m_panTilt	= CPanTiltClientProxy::newInstance();
	m_led 		= CLightEmittingDiodeClientProxy::newInstance();
	m_arm		= CArmClientProxy::newInstance();
	m_wheel		= CWheelClientProxy::newInstance();
	m_gyro		= CGyroClientProxy::newInstance();
	m_encoder	= CWheelEncoderClientProxy::newInstance();
}

DeviceConnector::~DeviceConnector() {
	if(m_panTilt) {
		delete m_panTilt;
		m_panTilt = NULL;
	}

	if(m_arm) {
		delete m_arm;
		m_arm = NULL;
	}

	if(m_wheel) {
		delete m_wheel;
		m_wheel = NULL;
	}

	if(m_led) {
		delete m_led;
		m_led = NULL;
	}

	if(m_gyro) {
		delete m_gyro;
		m_gyro = NULL;
	}

	if(m_encoder) {
		delete m_encoder;
		m_encoder = NULL;
	}
}

DeviceConnector* DeviceConnector::getInstance() {
	static DeviceConnector _thisInstance;
	return &_thisInstance;
}

CArm* DeviceConnector::getArm() {
	return m_arm;
}
CPanTilt* DeviceConnector::getPantilt() {
	return m_panTilt;
}
CWheel* DeviceConnector::getWheel() {
	return m_wheel;
}
CLightEmittingDiode* DeviceConnector::getLed() {
	return m_led;
}

CGyro* DeviceConnector::getGyro(){
	return m_gyro;
}
CWheelEncoder* DeviceConnector::getEncoder(){
	return m_encoder;
}
int DeviceConnector::getInitX() {
	return initX;
}
int DeviceConnector::getInitY() {
	return initY;
}
int DeviceConnector::getInitT() {
	return initT;
}

void DeviceConnector::setInitData(){
	CWheelEncoderData wData = m_encoder->readData(0);
	CGyroData gData = m_gyro->readData();
	initX = (int)wData.getX();
	initY = (int)wData.getY();
	initT = (int)gData.getAngle();
	ROS_DEBUG("Behavior Start Position : %d, %d, %d", initX, initY, initT);

}
void DeviceConnector::getCurrentData(int* currData){

	CWheelEncoderData wData = m_encoder->readData(0);
	CGyroData gData = m_gyro->readData();
	currData[0] = (int)wData.getX();
	currData[1] = (int)wData.getY();
	currData[2] = (int)gData.getAngle();
	ROS_DEBUG("Behavior Start Position : %d, %d, %d", currData[0], currData[1], currData[2]);

}
