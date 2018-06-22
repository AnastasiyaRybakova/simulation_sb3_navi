/*
 * MessageReceiver.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: ijpark
 */

#include <ros/ros.h>
#include <istream>
#include "MessageReceiver.h"
#include <avatar/message/MessageParser.h>

using namespace std;
using namespace avatar::message;

namespace avatar {
namespace transport {

MessageReceiver::MessageReceiver(WSocket* socket) {
	this->socket = socket;
}

MessageReceiver::~MessageReceiver() {
}

int MessageReceiver::readXML(string& buffer) throw (IOException){
	try {
		stringstream lines;
		string line;
		bool escape = false;
		do {
			readLine(line);

			for(int i = 0; i < line.length(); i++) {
				if(line.at(i) == '@') {
					escape = true;
				}
			}

			if(escape) {
				break;
			} else {
				lines << line << '\n';
			}
		} while(line != "@");

		buffer = lines.str();
		return buffer.length();

	} catch(roscir::exception::IOException& e) {
		cout << "IOException readXML" << endl;
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
	return 0;
}

int MessageReceiver::readLine(string& buffer) throw (IOException) {
	try {

		this->socket->readLine(buffer);

	} catch(roscir::exception::IOException& ioe) {
		cout << "IOException readLine" << endl;
		ioe.setMark(__FILE__, __LINE__);
//		ioe.setMessage(e.what());
		throw ioe;
	}
}


void MessageReceiver::getMessage(avatar::message::GameMessage& message)  throw (IOException){
	try {
		string xml;
		readXML(xml);

//		ROS_DEBUG("xml : %s", xml.c_str());

		MessageParser parser;
		parser.parse(xml.c_str(), &message);
	} catch(ParseException& pe) {
		cout << "ParseException getMessage" << endl;
		IOException e;
		e.setMark(__FILE__, __LINE__);
		e.setMessage("ParseException occured");
		throw e;
	}

}

} /* namespace transport */
} /* namespace avatar */
