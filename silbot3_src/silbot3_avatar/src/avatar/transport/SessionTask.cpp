/*
 * SessionTask.cpp
 *
 *  Created on: 2013. 7. 12.
 *      Author: ijpark
 */

#include "SessionTask.h"
#include "SessionMap.h"
#include <avatar/util/UUID.h>
#include <iostream>
#include <ros/ros.h>
#include <avatar/message/GameMessage.h>
#include <avatar/transport/MessageReceiver.h>
#include <avatar/transport/MessageWriter.h>
#include <avatar/util/UUID.h>

using namespace std;
using namespace avatar::util;

namespace avatar {
namespace transport {

SessionTask::SessionTask() {
	runningFlag = false;
	util::UUID uid;
	//this->name = uid.genRandom();
	//this->listener = NULL;
	this->socket = NULL;
}

SessionTask::SessionTask(WSocket* socket) {
	ROS_INFO("SessionTask::SessionTask");
	runningFlag = false;
	this->socket = socket;

	util::UUID uid;
	this->name = uid.genRandom();

}

SessionTask::~SessionTask() {

	stop();
	this->socket->closeSocket();
	delete this->socket;
	this->socket = NULL;
	ROS_DEBUG("SessionTask finished");
}

void SessionTask::run() {

	ROS_INFO("TASK RUN");

	runningFlag = true;
	int readSize = 0;
	string buffer;
	MessageReceiver receiver(this->socket);
	MessageWriter writer(this->socket);

	try {
		avatar::message::GameMessage message;
		receiver.getMessage(message);
		string sid = message.getSessionID();
		string url = message.getParameter("uri");
		string opname = message.getOperationName();

		if(opname == "regist") {
			this->name = url;
			SessionMap::getInstance()->setSession(this->name, this);

		} else {
			ROS_INFO("protocol invalid. all client must have registred sid to the server");
			IOException ioe;
			ioe.setMessage("IOException : protocol invalid. all client must have registred sid to the server");
			ioe.setMark(__FILE__, __LINE__);
			throw ioe;
		}

		while(runningFlag) {
			GameMessage receivedMessage;
			receiver.getMessage(receivedMessage);

			//this->write(message);
			//ROS_DEBUG("received message : \n %s", receivedMessage.toXML().c_str());
			if(receivedMessage.getOperationName() == "close") {
				break;
			} else if(receivedMessage.getOperationName() == "noti_avatar_viseme"){
				avatar::util::UUID uuid;
				string uid = uuid.genRandom();

				GameMessage bmessage;
				bmessage.setOperationName(receivedMessage.getOperationName());
				bmessage.setSessionID(uid);
				bmessage.addParameter("viseme", receivedMessage.getParameter("viseme"));

				receivedMessage.setSessionID(uid);
				broadcast(bmessage);
			} else if(receivedMessage.getOperationName() == "req_noti_avatar_mode") {
				avatar::util::UUID uuid;
				string uid = uuid.genRandom();

				GameMessage bmessage;
				bmessage.setOperationName("noti_avatar_mode");
				bmessage.setSessionID(uid);
				bmessage.addParameter("id", receivedMessage.getParameter("id"));
				broadcast(bmessage);
			} else if(receivedMessage.getOperationName() == "req_noti_avatar_exp") {
				avatar::util::UUID uuid;
				string uid = uuid.genRandom();

				GameMessage bmessage;
				bmessage.setOperationName("noti_avatar_exp");
				bmessage.setSessionID(uid);
				bmessage.addParameter("id", receivedMessage.getParameter("id"));
				broadcast(bmessage);
			}  else if(receivedMessage.getOperationName() == "req_noti_selected_menu") {
				avatar::util::UUID uuid;
				string uid = uuid.genRandom();

				GameMessage bmessage;
				bmessage.setOperationName("noti_selected_menu");
				bmessage.setSessionID(uid);
				bmessage.addParameter("id", receivedMessage.getParameter("id"));
				broadcast(bmessage);


			}  else if(receivedMessage.getOperationName() == "req_noti_avatar_face") {
				avatar::util::UUID uuid;
				string uid = uuid.genRandom();

				GameMessage bmessage;
				bmessage.setOperationName("noti_avatar_face");
				bmessage.setSessionID(uid);
				bmessage.addParameter("face", receivedMessage.getParameter("face"));
				bmessage.addParameter("duration", receivedMessage.getParameter("duration"));
				bmessage.addParameter("weight", receivedMessage.getParameter("weight"));
				broadcast(bmessage);


			}else {
				ROS_INFO("Cannot handle Message %s" ,receivedMessage.getOperationName().c_str() );
			}
			writer.sendMessage(receivedMessage);
		}
	} catch(exception& e) {
		cout << "SessionTask::run exception" << endl;
		ROS_ERROR("exception occured : %s", e.what());
	} catch(IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	} catch(NoSuchElementException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}
	runningFlag = false;
	SessionMap::getInstance()->remove(getName());

	// destruct self
	delete this;
}

void SessionTask::stop() {
	if(this->runningFlag) {
		this->runningFlag = false;
		this->thread.join();
	}
	ROS_DEBUG("thread : [%s] stopped", getName().c_str());
}

const boost::thread* SessionTask::getThread() const {
	return NULL;
}


void SessionTask::start() {
	this->thread = boost::thread(boost::bind(&SessionTask::run, this));
}

//void SessionTask::threadEntry() {
//	this->run();
//}
//
//void SessionTask::threadEntry(boost::thread* thread, tcp::socket* socket) {
//	this->socket = socket;
//	run();
//}

void SessionTask::setName(const std::string& name) {
	this->name = name;
}

const std::string& SessionTask::getName() const {
	return this->name;
}

void SessionTask::broadcast(GameMessage& message) {
	SessionTask* task = NULL;
	vector<string> keys = SessionMap::getInstance()->keySet();
	for(unsigned int i = 0; i < keys.size(); i++) {
		task = (SessionTask*)SessionMap::getInstance()->getSession(keys[i]);
		if(task != NULL) {
			task->writeMessage(message);
		}
	}
}

void SessionTask::writeMessage(GameMessage& message) {
//	ROS_DEBUG("write message : %s task", this->getName());
	MessageWriter writer(this->socket);
	writer.sendMessage(message);
}

//void SessionTask::setListener(SessionTaskListener* listener) {
//	this->listener = listener;
//}

} /* namespace transport */
} /* namespace avatar */
