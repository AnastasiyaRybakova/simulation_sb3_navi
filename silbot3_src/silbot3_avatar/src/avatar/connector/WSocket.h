#ifndef __WSOCK_H__MY_WINSOCK
#define __WSOCK_H__MY_WINSOCK

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <robocare/exception/IOException.h>

using namespace std;
class WSocket
{
private:
	int client;
	bool closed;
public:
	WSocket(void);
	WSocket(int socket);
	~WSocket(void);

public:
	int create();
	int connectSocket(const char*  ip, const int port);
	int closeSocket();

	void writeBodyFloat(char* buffer, int offset, float value);
	void writeBodyInt(char* buffer, int offset, int value);
	void writeBuffer(char*buffer, int size);

	int writeHeader(unsigned short id);

	char getCharFromBuffer(char* buffer, int offset, int nByte);
	unsigned short getUShortFromBuffer(char* buffer, int offset, int nByte);
	unsigned int getUIntFromBuffer(char* buffer, int offset, int nByte);
	int getIntFromBuffer(char* buffer, int offset, int nByte);
	float getFloatFromBuffer(char* buffer, int offset, int nByte);

	//int writeHeaderBody(unsigned short id, unsigned short size, int val1);
	//int writeHeaderBody(unsigned short id, unsigned short size, float val1, float val2, float val3);

	int read(void* buffer, const int& size);
	int readLine(string& buffer) throw (roscir::exception::IOException);
	int write(const char* buffer, const int& size);
	int write(const string& data);

};


#endif
