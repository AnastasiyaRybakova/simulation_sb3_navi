/*
 * AvatarServer.cpp
 *
 *  Created on: 2013. 7. 12.
 *      Author: ijpark
 */


#include "AvatarServer.h"
#include <ros/ros.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <avatar/util/UUID.h>
#include <avatar/transport/SessionTask.h>
#include <avatar/transport/SessionMap.h>
#include <avatar/connector/WServerSocket.h>
#include <avatar/connector/WSocket.h>
#include <sstream>
#include <robocare/exception/IOException.h>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

namespace avatar {
namespace server {

AvatarServer::AvatarServer() {
	runningFlag = false;
	playLipFlag = false;
	avatarMode = 0;
}

AvatarServer::~AvatarServer() {
}

void AvatarServer::initROSService(int argc, char **argv)
{
	ros::NodeHandle nh;
	ros::ServiceServer playLipSyncService = nh.advertiseService("/avatar_ros/play_lip_sync", &AvatarServer::playLipSyncCB, this);
	ros::ServiceServer exprssionService = nh.advertiseService("/avatar_ros/expression", &AvatarServer::expressionCB, this);
	ros::ServiceServer moveFaceService = nh.advertiseService("/avatar_ros/move_face", &AvatarServer::moveFaceCB, this);
	ros::ServiceServer faceFactorService = nh.advertiseService("/avatar_ros/set_face_factor", &AvatarServer::faceFactorCB, this);
	ros::ServiceServer playMocapSongService = nh.advertiseService("/avatar_ros/paly_mocap_song", &AvatarServer::mocapCB, this);
	ros::ServiceServer setWindowService = nh.advertiseService("/avatar_ros/set_window",  &AvatarServer::setWindowCB, this);
	ros::ServiceServer dialogService = nh.advertiseService("/avatar_ros/dialog",  &AvatarServer::dialogCB, this);
	ros::ServiceServer setMode = nh.advertiseService("/avatar_ros/set_mode",  &AvatarServer::setModeCB, this);
	ROS_INFO("[Avatar Service Server] Ready to Server");
	ros::spin();
}

void AvatarServer::startRosService(int argc, char **argv)
{
	this->rosserviceThread =  boost::thread(boost::bind(&AvatarServer::initROSService, this, argc, argv));
}

void AvatarServer::run() {
	ROS_INFO("avatar server started");
	runningFlag = true;

	try {
		ros::NodeHandle nh("~");
		std::string ip;
		int port;

		nh.param<std::string>("server_ip", ip, "127.0.0.1");
		nh.param<int>("server_port", port, 5000);

		ROS_INFO("AvatarServer IP: %s  PORT: %d", ip.c_str(), port);
		this->serversock.bind(ip, port);
		WSocket* client = NULL;
		while(runningFlag) {
			ROS_INFO("wait for new client... ");
			client = this->serversock.acceptSocket();
			handleClient(client);
		}
	} catch(IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	} catch(Exception& e){
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}

	this->worker.join();
	ROS_INFO("avatar server finished");

}

void AvatarServer::handleClient(WSocket* socket) {
	ROS_INFO("AvatarServer::handleClient... ");
	util::UUID uuid;
	string u = uuid.genRandom();
	SessionTask* sessionTask = new SessionTask(socket);
	sessionTask->start();
}


void AvatarServer::start() {
	this->worker = boost::thread(boost::bind(&AvatarServer::threadEntry, this));
}


void AvatarServer::startLipmessagesender(const vector<silbot3_msgs::TTSViseme> & visemes)
{
	this->message_sender_thread =  boost::thread(boost::bind(&AvatarServer::lipmessagesender, this, visemes));
}

void AvatarServer::lipmessagesender(const vector<silbot3_msgs::TTSViseme> & visemes)
{
		playLipFlag = false;
		for(unsigned int i = 0; i<visemes.size(); i++){

			if (playLipFlag){
				playLipFlag = false;
				broadcastVisemeMessage(0);
				break;
			}
			//ros::Time start = ros::Time::now();
			broadcastVisemeMessage(visemes[i].id);
			//ROS_DEBUG("res message : %s", resMessage->toXML().c_str());
			//ros::Time end = ros::Time::now();
			//ros::Duration ellapsed = end - start;
			//ROS_DEBUG("ellapsed time : %f", ellapsed.toSec());
			double iDuration =  (double)((visemes[i].duration));
            double dDuration = iDuration / 1000.0;
            ros::Duration d(dDuration);
            d.sleep();
        }

    }

void AvatarServer::broadcastVisemeMessage(int visemeID)
{
	GameMessage message;
	message.setOperationName("noti_avatar_viseme");
	stringstream ssID;
	ssID << visemeID;
	string strID = ssID.str();
	string visemeTime = strID + ",100000";
	message.addParameter("viseme", visemeTime);
	try {
	broadcast(message);
} catch(IOException& e) {
	e.setMark(__FILE__, __LINE__);
	e.printStackTrace();
	ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
}
}

void AvatarServer::wavPlaymessagesender(string path, string fileName)
{
//	GameMessage message;
//	message.setOperationName("noti_avatar_play_lip");
//	string fullpath = "";
//	if(path != ""){
//		fullpath = fileName;
//	}else{
//		fullpath = path + "/ " + fileName;
//	}
//	message.addParameter("lipwav", fullpath);
//	try {
//		broadcast(message);
//	} catch(IOException& e) {
//		e.setMark(__FILE__, __LINE__);
//		e.printStackTrace();
//		ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
//	}
//
//	cout << "Message: " << message.toXML().c_str()<< endl;
}



void AvatarServer::threadEntry(AvatarServer *parameter)
{
	parameter->run();
}

void AvatarServer::sessionStart(SessionTask *task)
{
	ROS_DEBUG("session started");
}

void AvatarServer::sessionFinish(SessionTask *task)
{
	ROS_DEBUG("session finish");
}

void AvatarServer::broadcast(GameMessage & message)
{
	vector<string> keys = SessionMap::getInstance()->keySet();
	SessionTask* session = NULL;
	for(unsigned int i =  0; i < keys.size(); i++) {
		session = (SessionTask*)SessionMap::getInstance()->getSession(keys[i]);
		try {
			session->writeMessage(message);
		} catch(roscir::exception::IOException& e) {
			e.setMark(__FILE__, __LINE__);
			e.printStackTrace();
			SessionMap::getInstance()->remove(keys[i]);
			delete session;
		}
	}
}

bool AvatarServer::playLipSyncCB(silbot3_msgs::AvatarRos_PlayLipSync_Srv::Request & request, silbot3_msgs::AvatarRos_PlayLipSync_Srv::Response & response)
{
	string opname = request.opname;
	string dirPath = request.dirPath;
	string fileName = request.filename;

	if(opname == "tts" || opname == "song"){
		vector<silbot3_msgs::TTSViseme> visemes = request.visemes;

		if(visemes.size() == 0){
			wavPlaymessagesender(dirPath, fileName);
		}else{
			startLipmessagesender(visemes);
		}
	}else if(opname == "stop"){
		if(playLipFlag == false){
			playLipFlag = true;
		}
	}else if(opname == "playWavLip"){
		wavPlaymessagesender(dirPath, fileName);
	}
	response.result = "OK";
	return true;
}

bool AvatarServer::expressionCB(silbot3_msgs::AvatarRos_Expression_Srv::Request & request, silbot3_msgs::AvatarRos_Expression_Srv::Response & response)
{
	string opname = request.opname;
	int duration = request.duartion;
	float weightf = request.weight;
	float gain[32] = {};
	string facialId;
	if(opname == "basic"){
		facialId = "BASIC";
	}else if(opname == "angry"){
		facialId = "ANGRY";
	}else if(opname == "funny"){
		facialId = "FUNNY";
	}else if(opname == "happy"){
		facialId = "HAPPY";
	}else  if(opname == "sad"){
		facialId = "SAD";
	}else  if(opname == "surprise"){
		facialId = "SURPRISE";
	}else  if(opname == "wink"){
		facialId = "WINK";
	}else  if(opname == "smile"){
		facialId = "SMILE";
	}else{
		 facialId = opname;
	}

	string sduration = lexical_cast<std::string>(duration);
	string sweight = lexical_cast<std::string>(weightf);
	GameMessage message;
	message.setOperationName("noti_avatar_face");
	message.addParameter("face", facialId);
	message.addParameter("duration", sduration);
	message.addParameter("weight", sweight);
	ROS_DEBUG("expressionCB send message \n%s", message.toXML().c_str());
	try {
		broadcast(message);
	} catch(IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
		ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
	}
//	cout << "opname: " << opname << " duration: " << duration << " weightf: " << weightf << endl;
	response.result = "OK";
	return true;
}

bool AvatarServer::moveFaceCB(silbot3_msgs::AvatarRos_MoveFace_Srv::Request & request, silbot3_msgs::AvatarRos_MoveFace_Srv::Response & response)
{
	string opname = request.opname;

	int lipid = 0;

	if(opname.compare("moutha") == 0){
		lipid = 2;
	}else if(opname.compare("mouthe") == 0){
		lipid = 4;
	}else if(opname.compare("moutho") == 0){
		lipid = 8;
	}else if(opname.compare("closemouth") == 0){
		lipid = 0;
	}else{
		lipid = 0;
	}

	GameMessage message;
	message.setOperationName("noti_avatar_viseme");
	stringstream ssID;
	ssID << lipid;
	string strID = ssID.str();
	string visemeTime = strID + ",100000";
	message.addParameter("viseme",visemeTime);
	try{
		broadcast(message);
	}catch (IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
		ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
	}


	response.result = "OK";
	return true;
}

bool AvatarServer::faceFactorCB( silbot3_msgs::AvatarRos_FaceFactor_Srv::Request &request, silbot3_msgs::AvatarRos_FaceFactor_Srv::Response &response )
{

	string opname = request.opname;
	if(opname.compare("blink")==0) {

	}else if(opname.compare("changeAvatar")==0){
		GameMessage message;

		string typeID;
		std::stringstream out;
		out <<  request.type;
		typeID = out.str();

		message.setOperationName("noti_avatar_face");
		message.addParameter("face", typeID);

		try {
			broadcast(message);
		} catch(IOException& e) {
			e.setMark(__FILE__, __LINE__);
			e.printStackTrace();
			ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
		}

	}
	response.result = "OK";
	return true;
}

bool AvatarServer::setWindowCB(silbot3_msgs::AvatarRos_SetWindow_Srv::Request & request, silbot3_msgs::AvatarRos_SetWindow_Srv::Response & response)
{
	string opname = request.opname;
	if(opname.compare("setWindow")==0) {

		int posX = request.posX;
		int posY= request.posY;
		int width = request.width;
		int height = request.height;

		GameMessage message;

		message.setOperationName("noti_avatar_set_window");
		message.addParameter("x", boost::lexical_cast<string>(posX));
		message.addParameter("y", boost::lexical_cast<string>(posY));
		message.addParameter("width", boost::lexical_cast<string>(width));
		message.addParameter("height", boost::lexical_cast<string>(height));

		try {
			broadcast(message);
		} catch(IOException& e) {
			e.setMark(__FILE__, __LINE__);
			e.printStackTrace();
			ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
		}

	}
	response.result = "OK";
	return true;
}

bool AvatarServer::mocapCB(silbot3_msgs::AvatarRos_PlayMocapSong_Srv::Request & request, silbot3_msgs::AvatarRos_PlayMocapSong_Srv::Response & response)
{

	string opname = request.opname;
	ROS_DEBUG("opname : %s\n", opname.c_str());

	if(opname.compare("playMocapSong")==0){
		if(avatarMode == 0) {
			GameMessage message2;

			message2.setOperationName("noti_avatar_mode_change");
			message2.addParameter("mode","2");

			try {
				broadcast(message2);
			} catch(IOException& e) {
				e.setMark(__FILE__, __LINE__);
				e.printStackTrace();
				ROS_DEBUG("IOException Occured. cannot send message \n %s", message2.toXML().c_str());
			}
			ROS_DEBUG("Send message \n %s", message2.toXML().c_str());

			ros::Duration(0.1).sleep();
			avatarMode = 2;
		}

		GameMessage message;

		string songPath = request.songDirPath + "\\" + request.songFilename;
		string mocapPath = request.mocapDirPath + "\\" + request.mocapFilename;

		ROS_DEBUG("songPath : %s\n", songPath.c_str());
		ROS_DEBUG("mocapPath : %s\n", mocapPath.c_str());


		message.setOperationName("noti_avatar_mocap");
		message.addParameter("song",songPath);
		message.addParameter("mocap",mocapPath);

		try {
			broadcast(message);
		} catch(IOException& e) {
			e.setMark(__FILE__, __LINE__);
			e.printStackTrace();
			ROS_DEBUG("IOException Occured. cannot send message \n %s", message.toXML().c_str());
		}
		ROS_DEBUG("Send message \n %s", message.toXML().c_str());

	}
	response.result = "OK";


	return true;
}

bool AvatarServer::dialogCB(silbot3_msgs::AvatarRos_Dialog_Srv::Request & request, silbot3_msgs::AvatarRos_Dialog_Srv::Response & response)
{
	GameMessage message;
	message.setOperationName("noti_avatar_dialog_control");

	if (request.open_state) {
		message.addParameter("state", "open");
	}
	else {
		message.addParameter("state", "close");
	}

	int iconType = 0 + request.icon_type;

	message.addParameter("icon", boost::lexical_cast<string>(iconType));
	message.addParameter("ment", request.ment);

  ROS_INFO("Send message \n %s", message.toXML().c_str());
	broadcast(message);

	response.result = "OK";

	return true;
}

bool AvatarServer::setModeCB( silbot3_msgs::AvatarRos_SetMode_Srv::Request &request, silbot3_msgs::AvatarRos_SetMode_Srv::Response &response ) {
	string opname = request.opname;
	string mode = request.mode;
	ROS_DEBUG("%s mode : %s\n", opname.c_str(), mode.c_str());

	if(opname.compare("setMode") == 0){

		if(mode.compare("0") == 0 && (avatarMode == 2 || avatarMode == 3)) {
			GameMessage message2;

			message2.setOperationName("noti_avatar_mode_change");
			message2.addParameter("mode","0");

			try {
				broadcast(message2);
			} catch(IOException& e) {
				e.setMark(__FILE__, __LINE__);
				e.printStackTrace();
				ROS_DEBUG("IOException Occured. cannot send message \n %s", message2.toXML().c_str());
			}
			ROS_DEBUG("Send message \n %s", message2.toXML().c_str());

			ros::Duration(0.5).sleep();
			avatarMode = 0;
		} else if(mode.compare("2") == 0 && avatarMode == 0) {

			GameMessage message2;

			message2.setOperationName("noti_avatar_mode_change");
			message2.addParameter("mode","2");

			try {
				broadcast(message2);
			} catch(IOException& e) {
				e.setMark(__FILE__, __LINE__);
				e.printStackTrace();
				ROS_DEBUG("IOException Occured. cannot send message \n %s", message2.toXML().c_str());
			}
			ROS_DEBUG("Send message \n %s", message2.toXML().c_str());

			ros::Duration(0.1).sleep();
			avatarMode = 2;
		}  else if(mode.compare("3") == 0 && avatarMode == 0) {
			avatarMode = 3;

			boost::thread(boost::bind(&AvatarServer::playRandomLipsync, this));
		}
	}


	return true;
}

void AvatarServer::playRandomLipsync() {
	ros::Rate rate = 5;
	while(avatarMode == 3) {
		GameMessage message2;

		message2.setOperationName("noti_avatar_viseme");
		message2.addParameter("viseme","2,10000");

		try {
			broadcast(message2);
		} catch(IOException& e) {
			e.setMark(__FILE__, __LINE__);
			e.printStackTrace();
			ROS_DEBUG("IOException Occured. cannot send message \n %s", message2.toXML().c_str());
		}

		rate.sleep();
	}
}

} /* namespace server */
} /* namespace avatar */
