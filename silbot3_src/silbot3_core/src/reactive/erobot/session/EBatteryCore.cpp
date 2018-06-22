/*
 * EBatteryCore.cpp
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */

#include "EBatteryCore.h"
#include <fstream>


ofstream fp;

int minVolt;
int maxVolt;

using namespace reactive::erobot::core;

EBatteryCore::EBatteryCore() {
	fp.open("C:/opt/device_erobot_batt.log");
	minVolt = 65535;
	maxVolt = -999;
	_this_battery_data = 0;
}

EBatteryCore::~EBatteryCore() {

}

void EBatteryCore::getBatteryCommandPacket(unsigned char command, unsigned char *buffer) {

	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE;
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checkSUM = 0x07;

	buffer[0] = EROBOT_DEVICE_BATTERY_ID;
	buffer[1] = EROBOT_DEVICE_BATTERY_SUB_ID_1;
	buffer[2] = EROBOT_DEVICE_BATTERY_SUB_ID_2;
	buffer[3] = command;		// 0xf8
	buffer[4] = checkSUM;
}

void EBatteryCore::addBatteryCommandPacket(unsigned char command, unsigned char *buffer, const int startIndex) {

	unsigned char checkSUM = 0x07;

	buffer[startIndex]		= EROBOT_DEVICE_BATTERY_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_BATTERY_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_BATTERY_SUB_ID_2;
	buffer[startIndex+3]	= command;
	buffer[startIndex+4]	= checkSUM;
}


int EBatteryCore::parseAndSetBatteryDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);

	if( (dataPacket[0] != 0x01)
		|| (dataPacket[1] != 0xF1)
		|| (dataPacket[2] != 0xF1)
		|| (dataPacket[3] != 0xF9) ) {
			//cout << "[EBatteryCore] res packet header is different! " << endl;
			//printf("\t received packet %02x %02x %02x %02x \n" ,dataPacket[0],dataPacket[1],dataPacket[2],dataPacket[3] );
			return RETURN_FAILED;
	}

//	if (checksum != dataPacket[dataPacketSize-1]) {
//		cout << "[EBatteryCore] checksum check fail... " << endl;
//		return RETURN_FAILED;
//	}

	// TODO check battery value
	_this_battery_data = (int) dataPacket[7];

	return RETURN_SUCCESS;
}

int EBatteryCore::parseAndSetBatteryDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksumWithStartIndex(dataPacket, dataPacketSize-1,startIndex);


//	if (checksum != dataPacket[startIndex+dataPacketSize-1]) {
//		cout << "[EBatteryCore] checksum check fail... " << endl;
//		return RETURN_FAILED;
//	}


	int nBuffHeaderOff = 4 + startIndex;

	int volt = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff,2);

	if(volt < minVolt) {
		fp << "min : " << volt << endl;
		minVolt = volt;
	} else if(volt > maxVolt) {
		fp << "max : " << volt << endl;
		maxVolt = volt;
	}

	_this_battery_data = (int) dataPacket[nBuffHeaderOff+2];
	return RETURN_SUCCESS;
}


void EBatteryCore::getBatteryValue(int* voltage) {
	voltage[0] = _this_battery_data;
}

bool EBatteryCore::isBatteryPacket(unsigned char* buffer, const int startIndex) {
	if( (buffer[startIndex] != 0x01)
		|| (buffer[startIndex+1] != 0xF1)
		|| (buffer[startIndex+2] != 0xF1)
		|| (buffer[startIndex+3] != 0xF9) ) {
			return false;
	}
	return true;
}


