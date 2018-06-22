#pragma once

#include <iostream>
#include <string>
#include "WSocket.h"

using namespace std;
int initwinsock();
class WServerSocket
{
private:
	int listeningSocket;
	string ip;
	int port;
	struct sockaddr_in server;
public:
	WServerSocket(void);
	~WServerSocket(void);

public:
	int bind(const string& ip, const int& port);
	WSocket* acceptSocket();
	
};
