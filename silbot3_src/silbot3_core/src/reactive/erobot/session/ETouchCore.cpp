#include "ETouchCore.h"

using namespace std;
using namespace reactive::erobot::core;

ETouchCore::ETouchCore() {
	memset(_this_touch_data, 0x00, sizeof(unsigned short) * EROBOT_TOUCH_DATACOUNT);
}

ETouchCore::~ETouchCore() {

}


void ETouchCore::getTouchButtonCommandPacket(unsigned char command, unsigned char* buffer) {
	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checkSUM = 0x0E;  // ~command

	buffer[0] = EROBOT_DEVICE_TOUCHBUTTON_ID;
	buffer[1] = EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_1;
	buffer[2] = EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_2;
	buffer[3] = command;
	buffer[4] = checkSUM;
}

void ETouchCore::addTouchButtonCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex) {

	unsigned char checkSUM = 0x0E;  // ~command

	buffer[startIndex]		= EROBOT_DEVICE_TOUCHBUTTON_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_2;
	buffer[startIndex+3]	= command;
	buffer[startIndex+4]	= checkSUM;
}

void ETouchCore::addTouchButtonErrorPacket(unsigned char command, unsigned char* buffer, const int touchId, const int startIndex) {

	unsigned char checkSUM = 0x0E;  // ~command

	buffer[startIndex]		= EROBOT_DEVICE_TOUCHBUTTON_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_TOUCHBUTTON_SUB_ID_2;
	buffer[startIndex+3]	= command;
	buffer[startIndex+4]	= touchId;
	buffer[startIndex+5]	= checkSUM;
}

int ETouchCore::parseAndSetTouchButtonDataPacket(int dataPacketSize, unsigned char* dataPacket) {
	
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);

	if( (dataPacket[0] != 0x01)
		|| (dataPacket[1] != 0xF1)
		|| (dataPacket[2] != 0xF1)
		|| (dataPacket[3] != 0xF6) ) {
//			cout << "[ETouchCore] res packet header is different! " << endl;
//			cout << "\t required packet 01 F1 F1 F6 " << endl;
//			printf("\t received packet %02x %02x %02x %02x \n" ,dataPacket[0],dataPacket[1],dataPacket[2],dataPacket[3] );
			return RETURN_FAILED;
	}
	int nBuffHeaderOff = 4;
	for (int i = nBuffHeaderOff ; i < dataPacketSize-1 ; i++) {
		_this_touch_data[i] = (unsigned short) dataPacket[i];
	}
	return RETURN_SUCCESS;
}

int ETouchCore::parseAndSetTouchButtonDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {
	
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksumWithStartIndex(dataPacket, dataPacketSize-1, startIndex);
	
	if (checksum != dataPacket[startIndex+dataPacketSize-1]) {
		cout << "[ETouchCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}

	int nBuffHeaderOff = startIndex+4;
	for (int i = 0 ; i < dataPacketSize-1 ; i++) {
		_this_touch_data[i] = (unsigned short) dataPacket[i+nBuffHeaderOff];
//		cout << "2222222222222222222222222222222222 i : " << i << " , " << _this_touch_data[i] << endl;
	}
	return RETURN_SUCCESS;
}

void ETouchCore::getTouchButtonValue(unsigned short *data, int nDataSize) {
	for (int i = 0 ; i < nDataSize; i++) {
		data[i] = _this_touch_data[i];
	}
}

bool ETouchCore::isTouchButtonPacket(unsigned char* buffer, const int startIndex) {
	if( (buffer[startIndex] != 0x01)
		|| (buffer[startIndex+1] != 0xF1)
		|| (buffer[startIndex+2] != 0xF1)
		|| (buffer[startIndex+3] != 0xF6) ) {
			return false;
	}
	return true;
}

bool ETouchCore::isTouchErrorPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF1)
		|| (buffer[2] != 0xF1)
		|| (buffer[3] != 0x0E) ) {
			return false;
	}
	return true;
}

int ETouchCore::parseAndSetTouchErrorDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 1;
	int nByteSize = 1;

	return ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
}
