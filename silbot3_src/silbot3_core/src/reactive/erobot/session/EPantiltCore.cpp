#include "EPantiltCore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
using namespace reactive::erobot::core;

EPantiltCore::EPantiltCore() {
	_current_pantilt_command = 0x00;
	memset(_current_pantilt_data, 0x00, sizeof(unsigned char) * EROBOT_PANTILT_DATA_PACKET_SIZE);
	utility = ERobotFunctionFactory::getInstance();
	panAngle = 0;
	tiltAngle = 0;
	xtionAngle = 0;
}


EPantiltCore::~EPantiltCore() {

}



void EPantiltCore::getPantiltCommandPacket(unsigned char *buffer) {
		
	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_PANTILT_DATA_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_pantilt_data, EROBOT_PANTILT_DATA_PACKET_SIZE);

	buffer[0]	= EROBOT_DEVICE_PANTILT_ID;
	buffer[1]	= EROBOT_DEVICE_PANTILT_SUB_ID_1;
	buffer[2]	= EROBOT_DEVICE_PANTILT_SUB_ID_2;
	buffer[3]	= _current_pantilt_command;
	//printf("---------------------\n");
	for(int i=0 ; i<EROBOT_PANTILT_DATA_PACKET_SIZE ; i++) {
		buffer[i+4] = _current_pantilt_data[i];
		//printf("%02x ", buffer[i+4]);
	}
	//printf("---------------------\n");
	buffer[13]	= checksum;
}

void EPantiltCore::addPantiltCommandPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_pantilt_data, EROBOT_PANTILT_DATA_PACKET_SIZE);
	
	buffer[startIndex]		= EROBOT_DEVICE_PANTILT_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_PANTILT_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_PANTILT_SUB_ID_2;
	buffer[startIndex+3]	= _current_pantilt_command;
	for(int i=0 ; i<EROBOT_PANTILT_DATA_PACKET_SIZE ; i++) {
		buffer[startIndex+i+4] = _current_pantilt_data[i];
	}
	buffer[startIndex+13]	= checksum;
}

void EPantiltCore::addGetPositionPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_pantilt_data, EROBOT_PANTILT_DATA_PACKET_SIZE);

	buffer[startIndex]		= EROBOT_DEVICE_PANTILT_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_PANTILT_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_PANTILT_SUB_ID_2;
	buffer[startIndex+3]	= EROBOT_COMMAND_PANTILT_GET_POSITION;
	for(int i=0 ; i<EROBOT_PANTILT_DATA_PACKET_SIZE ; i++) {
		buffer[startIndex+i+4] = 0;
	}
	buffer[startIndex+13]	= checksum;
}


int EPantiltCore::parseAndSetPantiltDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	unsigned char checksum = utility->getChecksum(dataPacket, dataPacketSize-1);

	if( (dataPacket[0] != 0x01)
		|| (dataPacket[1] != 0xF7)
		|| (dataPacket[2] != 0xF7)
		|| (dataPacket[3] != 0xF7) ) {
			//cout << "[EPantiltCore] res packet header is different! " << endl;
			return RETURN_FAILED;
	}
	
	if (checksum != dataPacket[dataPacketSize-1]) {
		cout << "[EPantiltCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}
	
	/*
	 *	dataPacket[4]~[9]   6byte
	 *	head-pan pose(degree)  2byte
	 *	head-tilt pose(degree)  2byte
	 *	xtion-tilt pose(degree)  2byte
	 */
	 
	
	return RETURN_SUCCESS;
}

int EPantiltCore::parseAndSetPantiltDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {
	
	/*
	 *	dataPacket[4]~[9]   6byte
	 *	head-pan pose(degree)  2byte
	 *	head-tilt pose(degree)  2byte
	 *	xtion-tilt pose(degree)  2byte
	 */

	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);

	int nBuffHeaderOff = 4+startIndex;
	int nByteLen  = 0;
	int nByteSize = 2;
	panAngle = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	tiltAngle = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	xtionAngle = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);


	return RETURN_SUCCESS;
}

void EPantiltCore::setPantiltValue(unsigned char id, short anglePan, short angleTilt,	
								   short angleXtionTilt, short speedPan, short speedTilt, short speenXtionTilt) {

	if(id == EROBOT_COMMAND_PANTILT_MOVE_TO_ABSOLUTE_POSITION) {
		_current_pantilt_command = EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL;
		utility->intTo2Bytes(anglePan,			_current_pantilt_data);
		utility->intTo2Bytes(angleTilt,			_current_pantilt_data+2);
		utility->intToByte(speedPan,			_current_pantilt_data+6);
		utility->intToByte(speedTilt,			_current_pantilt_data+7);
	} else if(id == EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABSOLUTE_POSITION) {
		_current_pantilt_command = EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL;
		utility->intTo2Bytes(angleXtionTilt,	_current_pantilt_data+4);
		utility->intToByte(speenXtionTilt,	_current_pantilt_data+8);
	} else if(id == EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL) {
		_current_pantilt_command = EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL;
		utility->intTo2Bytes(anglePan,			_current_pantilt_data);
		utility->intTo2Bytes(angleTilt,			_current_pantilt_data+2);
		utility->intTo2Bytes(angleXtionTilt,	_current_pantilt_data+4);
		utility->intToByte(speedPan,			_current_pantilt_data+6);
		utility->intToByte(speedTilt,			_current_pantilt_data+7);
		utility->intToByte(speenXtionTilt,	_current_pantilt_data+8);
	} else {
		_current_pantilt_command = id;
		utility->intTo2Bytes(anglePan,			_current_pantilt_data);
		utility->intTo2Bytes(angleTilt,			_current_pantilt_data+2);
		utility->intTo2Bytes(angleXtionTilt,	_current_pantilt_data+4);
		utility->intToByte(speedPan,			_current_pantilt_data+6);
		utility->intToByte(speedTilt,			_current_pantilt_data+7);
		utility->intToByte(speenXtionTilt,	_current_pantilt_data+8);
	}

}


void EPantiltCore::setHeadValue(unsigned char *values) {
	
		_current_pantilt_command = EROBOT_COMMAND_PANTILT_XTION_MOVE_TO_ABS_ALL;

		_current_pantilt_data[0] = values[4];
		_current_pantilt_data[1] = values[5];
		_current_pantilt_data[2] = values[6];
		_current_pantilt_data[3] = values[7];
		_current_pantilt_data[4] = values[8];
		_current_pantilt_data[5] = values[9];
		_current_pantilt_data[6] = values[10];
		_current_pantilt_data[7] = values[11];
		_current_pantilt_data[8] = values[12];

}

void EPantiltCore::getPantiltPositons(double* positions) {
	*positions = panAngle;
	*(positions+1) = tiltAngle;
	*(positions+2) = xtionAngle;
}

bool EPantiltCore::isPantiltPacket(unsigned char* buffer, const int startIndex) {

	if( (buffer[startIndex] != 0x01)
		|| (buffer[startIndex+1] != 0xF7)
		|| (buffer[startIndex+2] != 0xF7)
		|| (buffer[startIndex+3] != 0x0E) ) {
			return false;
	}
	return true;
}


bool EPantiltCore::isPantiltErrorPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF7)
		|| (buffer[2] != 0xF7)
		|| (buffer[3] != 0x0E) ) {
			return false;
	}
	return true;
}

int EPantiltCore::parseAndSetPantiltErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[3]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 1;

	for(int i=0; i<3; i++) {
		errorCode[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetPantiltErrorDataPacket codeIndex " << i << " is " << errorCode[i] << endl;
	}
	return RETURN_SUCCESS;
}

bool EPantiltCore::isPantiltPositionPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF7)
		|| (buffer[2] != 0xF7)
		|| (buffer[3] != 0xF7) ) {
			return false;
	}
	return true;
}

int EPantiltCore::parseAndSetPantiltPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, double angle[3]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 2;

	for(int i=0; i<3; i++) {
		angle[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
		//cout << "parseAndSetPantiltPositionDataPacket codeIndex " << i << " is " << angle[i] << endl;
	}
	angle[0] = -1*angle[0];
	return RETURN_SUCCESS;
}
