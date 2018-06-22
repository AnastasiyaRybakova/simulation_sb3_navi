#include "ELEDCore.h"

using namespace std;
using namespace reactive::erobot::core;

ELEDCore::ELEDCore() {
	_current_led_command = 0x00;
	memset(_current_led_data, 0x00, sizeof(unsigned char) * EROBOT_LED_DATA_PACKET_SIZE);
	utility = ERobotFunctionFactory::getInstance();
}

ELEDCore::~ELEDCore() {

}


void ELEDCore::getLEDCommandPacket( unsigned char* buffer) {
	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_LED_DATA_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_led_data, EROBOT_LED_DATA_PACKET_SIZE);
	// command
	// f2 both arm
	// f3 right arm
	// f4 left arm
	// f5 ring

	buffer[0] = EROBOT_DEVICE_LED_ID;
	buffer[1] = EROBOT_DEVICE_LED_SUB_ID_1;
	buffer[2] = EROBOT_DEVICE_LED_SUB_ID_2;
	buffer[3] = _current_led_command;
	for( int i=0 ; i<EROBOT_LED_DATA_PACKET_SIZE ; i++) {
		buffer[i+4] = _current_led_data[i];
	}
	buffer[14] = checksum;
}

void ELEDCore::addLEDCommandPacket( unsigned char* buffer, const int startIndex) {

	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_LED_DATA_PACKET_SIZE + nCheckSumSize;
	unsigned char checksum = utility->getChecksumWithNoOffset(_current_led_data, EROBOT_LED_DATA_PACKET_SIZE);

	buffer[startIndex] = EROBOT_DEVICE_LED_ID;
	buffer[startIndex+1] = EROBOT_DEVICE_LED_SUB_ID_1;
	buffer[startIndex+2] = EROBOT_DEVICE_LED_SUB_ID_2;
	buffer[startIndex+3] = _current_led_command;
	for( int i=0 ; i<EROBOT_LED_DATA_PACKET_SIZE ; i++) {
		buffer[startIndex+i+4] = _current_led_data[i];
	}
	buffer[startIndex+14] = checksum;
}

int ELEDCore::parseAndSetLEDDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	return RETURN_SUCCESS;
}

int ELEDCore::parseAndSetLEDDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {

	return RETURN_SUCCESS;
}

void ELEDCore::setLEDValue(unsigned char id, unsigned char red, unsigned char green, unsigned char blue, unsigned char blink) {
	_current_led_command = id;

	if(id == 0xF0) {
		_current_led_data[0] = red;
		_current_led_data[1] = green;
		_current_led_data[2] = blue;
		_current_led_data[3] = red;
		_current_led_data[4] = green;
		_current_led_data[5] = blue;
		_current_led_data[6] = red;
		_current_led_data[7] = green;
		_current_led_data[8] = blue;
		_current_led_data[9] = blink;
	} else if(id == 0xF5) {
		_current_led_data[0] = red;
		_current_led_data[1] = green;
		_current_led_data[2] = blue;
		_current_led_data[3] = 0;
		_current_led_data[4] = 0;
		_current_led_data[5] = 0;
		_current_led_data[6] = red;
		_current_led_data[7] = green;
		_current_led_data[8] = blue;
		_current_led_data[9] = blink;
	} else {
		_current_led_data[0] = red;
		_current_led_data[1] = green;
		_current_led_data[2] = blue;
		_current_led_data[3] = red;
		_current_led_data[4] = green;
		_current_led_data[5] = blue;
		_current_led_data[6] = 0;
		_current_led_data[7] = 0;
		_current_led_data[8] = 0;
		_current_led_data[9] = blink;
	}
}

bool ELEDCore::isLEDPacket(unsigned char* buffer, const int startIndex) {
	// can not check
	return false;
}
