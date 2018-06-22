/*
 * MessageWriter.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: ijpark
 */
#include <ros/ros.h>
#include "MessageWriter.h"
#include <sstream>

namespace avatar {
namespace transport {

MessageWriter::MessageWriter() {
}

MessageWriter::MessageWriter(WSocket* socket) {
	this->socket = socket;
}

MessageWriter::~MessageWriter() {
}

void MessageWriter::sendMessage(GameMessage& message) throw (IOException) {
	stringstream ss;
	ss<<"\n"<<message.toXML()<<"@\n";
	string xml = ss.str();

	int wsize = socket->write((const char*)xml.c_str(), xml.length());
	//ROS_DEBUG("MessageWriter : write message : %d", wsize);
}

} /* namespace connector */
} /* namespace avatar */
