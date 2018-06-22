#include "WSocket.h"
#include <stdio.h>
#include <ros/ros.h>

WSocket::WSocket(void)
{
	this->client = 0;
	this->closed = true;
}

WSocket::WSocket(int socket) {
	this->client = socket;
	this->closed = false;
}

WSocket::~WSocket(void)
{
	if(!closed) {
		closeSocket();
	}
}

int WSocket::create() {
	return 1;
}

int WSocket::connectSocket(const char* ip, const int port) {
	
	/*this->client = socket( AF_INET, SOCK_STREAM, 0 );
	struct hostent host;
	host = gethostbyname(ip.c_str());
	struct sockaddr_in sin;
	memset( &sin, 0, sizeof sin );
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
	sin.sin_point = htons(port);
	if ( connect( client, &sin, sizeof sin ) == SOCKET_ERROR )
	{
		return FALSE;
	}*/

    struct sockaddr_in server;

    //Create socket
    client = socket(AF_INET , SOCK_STREAM , 0);
    if (client == -1)
    {
    	ROS_ERROR("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(client , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
    	ROS_ERROR("connect error");
        return false;
    }
    ROS_INFO(" connect WSocket - ip : %s , port :%d",ip , port );
	return true;
}

int WSocket::closeSocket() {
	closed = true;
	return close(client);
}

int WSocket::writeHeader(unsigned short id) {

	return 1;
}

void WSocket::writeBodyFloat(char* buffer, int offset, float value)
{
	memcpy((void *)(buffer+offset), (void *)&value, 4);
}

void WSocket::writeBodyInt(char* buffer, int offset, int value)
{
	memcpy((void *)(buffer+offset), (void *)&value, 4);
}

void WSocket::writeBuffer(char* buffer, int size)
{
   if( send(client , buffer , strlen(buffer) , 0) < 0)
    {
        puts("Send failed");
    }
    puts("Data Send\n");

	/*
	printf("buffer [%d]  header : ", size );
	for (int i = 0 ; i < size ; i++) {
		if (i > 0) printf (":");
		if (i == sizeof(SIMUL_HEADER)) {
			printf("\n Body [%d] : ", size-sizeof(SIMUL_HEADER));
		}
		printf("%02X", buffer[i]);
	}
	printf("\n");
	*/
}


int WSocket::getIntFromBuffer(char *buffer, int offset, int nByte)
{
	int nRet = -1;
	

	return nRet;
}

float WSocket::getFloatFromBuffer(char *buffer, int offset, int nByte)
{
	int nRet = -1;

	return nRet;
}


int WSocket::read(void *buffer, const int& size) {
	int rdsize = recv(this->client, (char *)buffer, size, 0);
	return rdsize;
}

int WSocket::write(const char* buffer, const int& size) {
	int wdsize = send(this->client, buffer, size, 0);
	return wdsize;
}

int WSocket::write(const string& data) {
	int size = data.length();
	const char* str = data.c_str();
	int wdsize = this->write(str, size);
	return wdsize;
}

int WSocket::readLine(string& buffer) throw (roscir::exception::IOException) {
	string line = "";
	stringstream ss;
	char c;
	int count = 0;
	try {
		do {
//			cout << "C " << count  << " : "<< c << endl;
			int readSize = read((unsigned char*)&c, 1);
			if(readSize == 0) {
				count++;
			}
			if(c != '\n') {
				ss<<c;
			}
			if(count >=1000) {
				ss << "\n@";
				break;
			}
		} while(c != '\n');

		//line = ss.str();
		buffer = ss.str();

	} catch(roscir::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		throw e;
	}

	return buffer.length();
}
