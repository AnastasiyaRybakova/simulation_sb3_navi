#include "WServerSocket.h"
#include <ros/ros.h>

int initwinsock() {

	//Create socket

	return 1;
}

WServerSocket::WServerSocket(void)
{
	this->ip = "127.0.0.1";
	this->port = 5000;
	listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

WServerSocket::~WServerSocket(void)
{
}

int WServerSocket::bind(const string& ip, const int& port) {
	this->ip = ip;
	this->port = port;
	
	listeningSocket = socket(AF_INET,
		SOCK_STREAM,   	// This is a stream-oriented socket
		IPPROTO_TCP);		// Use TCP rather than UDP
	int nret = 0;
	if (listeningSocket == -1)
	{
		ROS_ERROR("Could not create socket");
		return -1;
	}
	
	listeningSocket = socket(AF_INET , SOCK_STREAM , 0);
	if (listeningSocket == -1)
	{
		ROS_ERROR("Could not create socket");
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr =  inet_addr(ip.c_str());
	server.sin_port = htons( port );

	//Bind
	if( ::bind(listeningSocket,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		ROS_ERROR("bind failed");
	}
	ROS_INFO("bind done");
	
	int iRet = listen(listeningSocket,10); // 대기큐 를 5로 한정한다.

	if(0 > iRet) // 에러발생시
	{
		ROS_ERROR("listen failed ");
	}
	ROS_INFO("listen done");
	return 1;
	
}

WSocket* WServerSocket::acceptSocket() {
	struct sockaddr client;
	int c = sizeof(struct sockaddr_in);
	int aclient = accept(listeningSocket, (struct sockaddr *)&client, (socklen_t*)&c);
	if (aclient<0)
	{
		ROS_ERROR("accept failed");
		return NULL;
	}

	WSocket* wsock = new WSocket(aclient);
	return wsock;
}

