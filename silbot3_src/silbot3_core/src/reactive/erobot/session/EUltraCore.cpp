#include "EUltraCore.h"

using namespace std;
using namespace reactive::erobot::core;

EUltraCore::EUltraCore() {
	memset(_this_ultrasonic_data, 0x00, sizeof(unsigned short)	* (EROBOT_ULTRASONIC_DATACOUNT+4)); // read 12 so...''
}

EUltraCore::~EUltraCore() {

}

void EUltraCore::getUltrsonicCommandPacket(unsigned char command, unsigned char *buffer) {

	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checkSUM = 0x0A;   // ~(command)

	buffer[0] = EROBOT_DEVICE_ULTRASONIC_ID;
	buffer[1] = EROBOT_DEVICE_ULTRASONIC_SUB_ID_1;
	buffer[2] = EROBOT_DEVICE_ULTRASONIC_SUB_ID_2;
	buffer[3] = command;
	buffer[4] = checkSUM;
}

void EUltraCore::addUltrsonicCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex) {

	unsigned char checkSUM = 0x0A;   // ~(command)

	buffer[startIndex]		= EROBOT_DEVICE_ULTRASONIC_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_ULTRASONIC_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_ULTRASONIC_SUB_ID_2;
	buffer[startIndex+3]	= command;
	buffer[startIndex+4]	= checkSUM;
}

int EUltraCore::parseAndSetUltrsonicDataPacket(int dataPacketSize, unsigned char* dataPacket) {
	
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);
	
	if( (dataPacket[0] != 0x01)
		|| (dataPacket[1] != 0xF2)
		|| (dataPacket[2] != 0xF2)
		|| (dataPacket[3] != 0xF5) ) {
		cout << "[EUltraCore] res packet header is different! " << endl;
		cout << "\t required packet 01 F2 F2 F5 " << endl;
		printf("\t received packet %02x %02x %02x %02x \n" ,dataPacket[0],dataPacket[1],dataPacket[2],dataPacket[3] );
		return RETURN_FAILED;
	}

	if (checksum != dataPacket[dataPacketSize-1]) {
		cout << "[EUltraCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}

	int nBuffHeaderOff = 4;
	for (int i = nBuffHeaderOff ; i < dataPacketSize-1 ; i++) {
		_this_ultrasonic_data[i-4] = (unsigned short) dataPacket[i];
	}
	return RETURN_SUCCESS;
}

int EUltraCore::parseAndSetUltrsonicDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {
	
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksumWithStartIndex(dataPacket, dataPacketSize-1, startIndex);

	if (checksum != dataPacket[startIndex+dataPacketSize-1]) {
		cout << "[EUltraCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}

	int nBuffHeaderOff = 4+startIndex;
	
	for (int i = 0 ; i < dataPacketSize-1 ; i++) {
		_this_ultrasonic_data[i] = (unsigned short) dataPacket[i+nBuffHeaderOff];
	}
	return RETURN_SUCCESS;
}

void EUltraCore::getUltrasonicValue(unsigned short *data, int nDataSize) {
	for (int i = 0 ; i < nDataSize; i++) {
		if(_this_ultrasonic_data[i]==0) {
			data[i] = 155;
		} else {
			data[i] = _this_ultrasonic_data[i];
		}
	}
}

bool EUltraCore::isUltrasonicPacket(unsigned char* buffer, const int startIndex) {
	if( (buffer[startIndex] != 0x01)
		|| (buffer[startIndex+1] != 0xF2)
		|| (buffer[startIndex+2] != 0xF2)
		|| (buffer[startIndex+3] != 0xF5) ) {
			return false;
	}
	return true;
}
