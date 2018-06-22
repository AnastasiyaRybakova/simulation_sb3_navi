/*
 * EBodyCore.cpp
 *
 *  Created on: 2016. 1. 26.
 *      Author: zikprid
 */

#include "EBodyCoreXtion.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;
using namespace reactive::erobot::core;

EBodyCoreXtion::EBodyCoreXtion() {
	_current_body_command = 0x00;
	memset(_current_body_data, 0x00, sizeof(unsigned char) * EROBOT_BODY_DATA_PACKET_SIZE2);
//	memset(_current_body_position, 0x00, sizeof(short) * 6);
	utility = ERobotFunctionFactory::getInstance();

}

EBodyCoreXtion::~EBodyCoreXtion() {

}

void EBodyCoreXtion::getBodyCommandPacket(unsigned char *buffer) {

	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE;
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_BODY_DATA_PACKET_SIZE2 + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_body_data, EROBOT_BODY_DATA_PACKET_SIZE2);

	buffer[0]	= EROBOT_DEVICE_BODY_ID;
	buffer[1]	= EROBOT_DEVICE_BODY_SUB_ID_1;
	buffer[2]	= EROBOT_DEVICE_BODY_SUB_ID_2;
	buffer[3]	= _current_body_command;

	for(int i=0 ; i<EROBOT_BODY_DATA_PACKET_SIZE2 ; i++) {
		buffer[i+4] = _current_body_data[i];
	}
	buffer[EROBOT_BODY_DATA_PACKET_SIZE2+EROBOT_COMMON_HEADER_PACKET_SIZE]	= checksum;
}

void EBodyCoreXtion::addBodyCommandPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_body_data, EROBOT_BODY_DATA_PACKET_SIZE2);

	buffer[startIndex]		= EROBOT_DEVICE_BODY_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_BODY_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_BODY_SUB_ID_2;
	buffer[startIndex+3]	= _current_body_command;
	for(int i=0 ; i<EROBOT_BODY_DATA_PACKET_SIZE2 ; i++) {
		buffer[startIndex+i+EROBOT_COMMON_HEADER_PACKET_SIZE] = _current_body_data[i];
	}
	buffer[startIndex+EROBOT_BODY_DATA_PACKET_SIZE2+EROBOT_COMMON_HEADER_PACKET_SIZE]	= checksum;
}


void EBodyCoreXtion::addGetPositionPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_body_data, EROBOT_BODY_DATA_PACKET_SIZE2);

	buffer[startIndex]		= EROBOT_DEVICE_BODY_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_BODY_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_BODY_SUB_ID_2;
	buffer[startIndex+3]	= EROBOT_COMMAND_BODY_GET_POSITION;
	for(int i=0 ; i<EROBOT_BODY_DATA_PACKET_SIZE2 ; i++) {
		buffer[startIndex+i+EROBOT_COMMON_HEADER_PACKET_SIZE] = 0;
	}
	buffer[startIndex+EROBOT_BODY_DATA_PACKET_SIZE2+EROBOT_COMMON_HEADER_PACKET_SIZE]	= checksum;
}

int EBodyCoreXtion::parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	unsigned char checksum = utility->getChecksum(dataPacket, dataPacketSize-1);

	if (checksum != dataPacket[dataPacketSize-1]) {
		return RETURN_FAILED;
	}

	/*
	 *	dataPacket[4]~[21]   12byte
	 *	body pose right*3	2byte
	 *	body pose left*3	2byte
	 */

	int nBuffHeaderOff = EROBOT_COMMON_HEADER_PACKET_SIZE;
	int nByteLen  = 0;
	int nByteSize = 2;
	_current_body_position[0] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[1] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[2] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[3] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[4] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[5] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);

	return RETURN_SUCCESS;
}

int EBodyCoreXtion::parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {

	/*
	 *	dataPacket[4]~[21]   12byte
	 *	body pose right*3	2byte
	 *	body pose left*3	2byte
	 */

	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);


	int nBuffHeaderOff = 4+startIndex;
	int nByteLen  = 0;
	int nByteSize = 2;
	_current_body_position[0] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[1] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[2] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[3] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[4] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[5] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[6] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
	nByteLen += nByteSize;
	_current_body_position[7] = (short)ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);

	return RETURN_SUCCESS;
}

void EBodyCoreXtion::setCommandValue(short values) {
	_current_body_command = values;
}


void EBodyCoreXtion::setLeftValue(short *values) {

	utility->intTo2Bytes(values[0],			_current_body_data+6);
	utility->intTo2Bytes(values[1],			_current_body_data+8);
	utility->intTo2Bytes(values[2],			_current_body_data+10);
	utility->intToByte(values[3],			_current_body_data+21);
	utility->intToByte(values[4],			_current_body_data+22);
	utility->intToByte(values[5],			_current_body_data+23);
}

void EBodyCoreXtion::setRightValue(short *values) {

	utility->intTo2Bytes(values[0],			_current_body_data);
	utility->intTo2Bytes(values[1],			_current_body_data+2);
	utility->intTo2Bytes(values[2],			_current_body_data+4);
	utility->intToByte(values[3],			_current_body_data+18);
	utility->intToByte(values[4],			_current_body_data+19);
	utility->intToByte(values[5],			_current_body_data+20);
}

void EBodyCoreXtion::setHeadValue(unsigned char *values) {

//	utility->intTo2Bytes(anglePan,			_current_pantilt_data+4);
//	utility->intTo2Bytes(angleTilt,			_current_pantilt_data+6);
//	utility->intToByte(speedPan,			_current_pantilt_data+10);
//	utility->intToByte(speedTilt,			_current_pantilt_data+11);

	_current_body_data[12] = values[4];
	_current_body_data[13] = values[5];
	_current_body_data[14] = values[6];
	_current_body_data[15] = values[7];
	_current_body_data[16] = values[8];
	_current_body_data[17] = values[9];
	_current_body_data[24] = values[10];
	_current_body_data[25] = values[11];
	_current_body_data[26] = values[12];

//	utility->intTo2Bytes(values[4],			_current_body_data+12);
//	utility->intTo2Bytes(values[6],			_current_body_data+14);
//	utility->intToByte(values[10],			_current_body_data+22);
//	utility->intToByte(values[11],			_current_body_data+23);
}

void EBodyCoreXtion::setHeadValueZero() {

	utility->intTo2Bytes(0,			_current_body_data+12);
	utility->intTo2Bytes(0,			_current_body_data+14);
	utility->intTo2Bytes(0,			_current_body_data+16);
	utility->intToByte(0,			_current_body_data+24);
	utility->intToByte(0,			_current_body_data+25);
	utility->intToByte(0,			_current_body_data+26);
}

bool EBodyCoreXtion::isBodyPacket(unsigned char* buffer, const int startIndex) {

	if( (buffer[startIndex] != 0x01)
		|| (buffer[1+startIndex] != 0xF8)
		|| (buffer[2+startIndex] != 0xF8)
		|| (buffer[3+startIndex] != 0xF8) ) {
			return false;
	}
	return true;
}

void EBodyCoreXtion::getBodyPositons(short* positions) {
	for(int i=0; i<6; i++) {
		positions[i] = _current_body_position[i];
	}
}


bool EBodyCoreXtion::isBodyErrorPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF8)
		|| (buffer[2] != 0xF8)
		|| (buffer[3] != 0x0E) ) {
			return false;
	}
	return true;
}

int EBodyCoreXtion::parseAndSetBodyErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[8]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 1;

	for(int i=0; i<8; i++) {
		errorCode[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetBodyErrorDataPacket codeIndex " << i << " is " << errorCode[i] << endl;
	}
	return RETURN_SUCCESS;
}

bool EBodyCoreXtion::isBodyPositionPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF8)
		|| (buffer[2] != 0xF8)
		|| (buffer[3] != 0xF8) ) {
			return false;
	}
	return true;
}

int EBodyCoreXtion::parseAndSetBodyPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, short angle[8]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 2;

	for(int i=0; i<8; i++) {
		angle[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetBodyPositionDataPacket angleIndex " << i << " is " << angle[i] << endl;
	}
	return RETURN_SUCCESS;
}
