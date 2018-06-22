#include "EArmCore.h"

using namespace std;
using namespace reactive::erobot::core;

EArmCore::EArmCore() {
	_current_arm_command = 0x00;
	memset(_current_arm_data, 0x00, sizeof(unsigned char) * EROBOT_ARM_DATA_PACKET_SIZE);
//	memset(_current_arm_position, 0x00, sizeof(short) * 6);
	utility = ERobotFunctionFactory::getInstance();

}

EArmCore::~EArmCore() {

}





void EArmCore::getArmCommandPacket(unsigned char *buffer) {
		
	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_ARM_DATA_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_arm_data, EROBOT_ARM_DATA_PACKET_SIZE);

	buffer[0]	= EROBOT_DEVICE_ARM_ID;
	buffer[1]	= EROBOT_DEVICE_ARM_SUB_ID_1;
	buffer[2]	= EROBOT_DEVICE_ARM_SUB_ID_2;
	buffer[3]	= _current_arm_command;

	for(int i=0 ; i<EROBOT_ARM_DATA_PACKET_SIZE ; i++) {
		buffer[i+4] = _current_arm_data[i];
	}
	buffer[22]	= checksum;
}

void EArmCore::addArmCommandPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_arm_data, EROBOT_ARM_DATA_PACKET_SIZE);
	
	buffer[startIndex]		= EROBOT_DEVICE_ARM_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_ARM_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_ARM_SUB_ID_2;
	buffer[startIndex+3]	= _current_arm_command;
	for(int i=0 ; i<EROBOT_ARM_DATA_PACKET_SIZE ; i++) {
		buffer[startIndex+i+4] = _current_arm_data[i];
	}
	buffer[startIndex+22]	= checksum;
}


void EArmCore::addGetPositionPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_arm_data, EROBOT_ARM_DATA_PACKET_SIZE);

	buffer[startIndex]		= EROBOT_DEVICE_ARM_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_ARM_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_ARM_SUB_ID_2;
	buffer[startIndex+3]	= EROBOT_COMMAND_ARM_GET_POSITION;
	for(int i=0 ; i<EROBOT_ARM_DATA_PACKET_SIZE ; i++) {
		buffer[startIndex+i+4] = 0;
	}
	buffer[startIndex+22]	= checksum;
}

int EArmCore::parseAndSetArmDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	unsigned char checksum = utility->getChecksum(dataPacket, dataPacketSize-1);
	
	if (checksum != dataPacket[dataPacketSize-1]) {
		return RETURN_FAILED;
	}
	
	/*
	 *	dataPacket[4]~[21]   12byte
	 *	arm pose right*3	2byte
	 *	arm pose left*3	2byte
	 */
	 
	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 2;
	_current_arm_position[0] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[1] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[2] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[3] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[4] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[5] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);

	return RETURN_SUCCESS;
}

int EArmCore::parseAndSetArmDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {
	
	/*
	 *	dataPacket[4]~[21]   12byte
	 *	arm pose right*3	2byte
	 *	arm pose left*3	2byte
	 */

	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);


	int nBuffHeaderOff = 4+startIndex;
	int nByteLen  = 0;
	int nByteSize = 2;
	_current_arm_position[0] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[1] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[2] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[3] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[4] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_arm_position[5] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);

	return RETURN_SUCCESS;
}

void EArmCore::setArmValue(unsigned char command, short *angles, short *speeds) {
	_current_arm_command = command;

	if(command == EROBOT_COMMAND_ARM_MOVE_RIGHT_ARM) {
		//cout << "EROBOT_COMMAND_ARM_MOVE_RIGHT_ARM " << endl;
		_current_arm_command = EROBOT_COMMAND_ARM_MOVE_BOTH_ARM;
		utility->intTo2Bytes(angles[0],			_current_arm_data);
		utility->intTo2Bytes(angles[1],			_current_arm_data+2);
		utility->intTo2Bytes(angles[2],			_current_arm_data+4);
		utility->intToByte(speeds[0],			_current_arm_data+12);
		utility->intToByte(speeds[1],			_current_arm_data+13);
		utility->intToByte(speeds[2],			_current_arm_data+14);
	} else if(command == EROBOT_COMMAND_ARM_MOVE_LEFT_ARM) {
		//cout << "EROBOT_COMMAND_ARM_MOVE_LEFT_ARM " << endl;
		_current_arm_command = EROBOT_COMMAND_ARM_MOVE_BOTH_ARM;
		utility->intTo2Bytes(angles[3],			_current_arm_data+6);
		utility->intTo2Bytes(angles[4],			_current_arm_data+8);
		utility->intTo2Bytes(angles[5],			_current_arm_data+10);
		utility->intToByte(speeds[3],			_current_arm_data+15);
		utility->intToByte(speeds[4],			_current_arm_data+16);
		utility->intToByte(speeds[5],			_current_arm_data+17);
	} else {
		utility->intTo2Bytes(angles[0],			_current_arm_data);
		utility->intTo2Bytes(angles[1],			_current_arm_data+2);
		utility->intTo2Bytes(angles[2],			_current_arm_data+4);
		utility->intTo2Bytes(angles[3],			_current_arm_data+6);
		utility->intTo2Bytes(angles[4],			_current_arm_data+8);
		utility->intTo2Bytes(angles[5],			_current_arm_data+10);

		utility->intToByte(speeds[0],			_current_arm_data+12);
		utility->intToByte(speeds[1],			_current_arm_data+13);
		utility->intToByte(speeds[2],			_current_arm_data+14);
		utility->intToByte(speeds[3],			_current_arm_data+15);
		utility->intToByte(speeds[4],			_current_arm_data+16);
		utility->intToByte(speeds[5],			_current_arm_data+17);
	}
//	cout << angles[0] << ", " << angles[1] << ", " <<angles[2] << ", " <<angles[3] << ", " <<angles[4] << ", " <<angles[5] << endl;
}


void EArmCore::setLeftValue(short *values) {

	_current_arm_command = EROBOT_COMMAND_ARM_MOVE_BOTH_ARM;
	utility->intTo2Bytes(values[0],			_current_arm_data+6);
	utility->intTo2Bytes(values[1],			_current_arm_data+8);
	utility->intTo2Bytes(values[2],			_current_arm_data+10);
	utility->intToByte(values[3],			_current_arm_data+15);
	utility->intToByte(values[4],			_current_arm_data+16);
	utility->intToByte(values[5],			_current_arm_data+17);
}

void EArmCore::setRightValue(short *values) {

	_current_arm_command = EROBOT_COMMAND_ARM_MOVE_BOTH_ARM;
	utility->intTo2Bytes(values[0],			_current_arm_data);
	utility->intTo2Bytes(values[1],			_current_arm_data+2);
	utility->intTo2Bytes(values[2],			_current_arm_data+4);
	utility->intToByte(values[3],			_current_arm_data+12);
	utility->intToByte(values[4],			_current_arm_data+13);
	utility->intToByte(values[5],			_current_arm_data+14);
}

bool EArmCore::isArmPacket(unsigned char* buffer, const int startIndex) {

	if( (buffer[startIndex] != 0x01)
		|| (buffer[1+startIndex] != 0xF8)
		|| (buffer[2+startIndex] != 0xF8)
		|| (buffer[3+startIndex] != 0xF8) ) {
			return false;
	}
	return true;
}

void EArmCore::getArmPositons(short* positions) {
	for(int i=0; i<6; i++) {
		positions[i] = _current_arm_position[i];
	}
}


bool EArmCore::isArmErrorPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF8)
		|| (buffer[2] != 0xF8)
		|| (buffer[3] != 0x0E) ) {
			return false;
	}
	return true;
}

int EArmCore::parseAndSetArmErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[6]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 1;

	for(int i=0; i<6; i++) {
		errorCode[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetArmErrorDataPacket codeIndex " << i << " is " << errorCode[i] << endl;
	}
	return RETURN_SUCCESS;
}

bool EArmCore::isArmPositionPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF8)
		|| (buffer[2] != 0xF8)
		|| (buffer[3] != 0xF8) ) {
			return false;
	}
	return true;
}

int EArmCore::parseAndSetArmPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, short angle[6]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 2;

	for(int i=0; i<6; i++) {
		angle[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetArmPositionDataPacket angleIndex " << i << " is " << angle[i] << endl;
	}
	return RETURN_SUCCESS;
}
