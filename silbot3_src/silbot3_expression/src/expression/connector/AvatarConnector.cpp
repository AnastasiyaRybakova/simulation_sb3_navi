/*
 * AvatarConnector.cpp
 *
 *  Created on: 2014. 7. 9.
 *      Author: ADmin
 */

#include "AvatarConnector.h"
#include <ros/service.h>

namespace expression {
namespace connector {

AvatarConnector::AvatarConnector() {
	this->playMocapClient = nh.serviceClient<silbot3_msgs::AvatarRos_PlayMocapSong_Srv>("/avatar_ros/paly_mocap_song");
	this->setModeClient = nh.serviceClient<silbot3_msgs::AvatarRos_SetMode_Srv>("/avatar_ros/set_mode");
	this->expressionClient = nh.serviceClient<silbot3_msgs::AvatarRos_Expression_Srv>("/avatar_ros/expression");
	this->playLipsyncSrv = nh.serviceClient<silbot3_msgs::AvatarRos_PlayLipSync_Srv>("/avatar_ros/play_lip_sync", false);
}

AvatarConnector::~AvatarConnector() {
}

AvatarConnector* AvatarConnector::getInstance() {
	static AvatarConnector _thisInstance;
	return &_thisInstance;
}

void AvatarConnector::playMocap(const string& songDir, const string& songFile, const string& mocapDir, const string& mocapFile) throw (expression::exception::IOException) {
	silbot3_msgs::AvatarRos_PlayMocapSong_Srv srv;
	srv.request.opname = "playMocapSong";
	srv.request.mocapDirPath = mocapDir;
	srv.request.mocapFilename = mocapFile;
	srv.request.songDirPath = songDir;
	srv.request.songFilename = songFile;

	ROS_INFO("wating for service avaliable[/avatar_ros/paly_mocap_song]");
	bool success = ros::service::waitForService("/avatar_ros/paly_mocap_song");
	if(!success) {
		ROS_ERROR("service is not availabe..[/avatar_ros/paly_mocap_song]");
	} else {
		ROS_DEBUG("service is avaliable...[/avatar_ros/paly_mocap_song]");
	}

	if(!this->playMocapClient.call(srv)) {
		expression::exception::IOException e;
		e.setMessage("IOException : cannot call service [/avatar_ros/paly_mocap_song]");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
	ROS_INFO("success call service [/avatar_ros/paly_mocap_song]");
}

void AvatarConnector::setMode(const string& mode) throw (expression::exception::IOException) {
	silbot3_msgs::AvatarRos_SetMode_Srv srv;
	srv.request.opname = "setMode";
	srv.request.mode = mode;

	ROS_INFO("wating for service avaliable[/avatar_ros/set_mode]");
	bool success = ros::service::waitForService("/avatar_ros/set_mode");
	if(!success) {
		ROS_ERROR("service is not availabe..[/avatar_ros/set_mode]");
	} else {
		ROS_DEBUG("service is avaliable...[/avatar_ros/set_mode]");
	}

	if(!this->setModeClient.call(srv)) {
		expression::exception::IOException e;
		e.setMessage("IOException : cannot call service [/avatar_ros/set_mode]");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
	ROS_INFO("success call service [/avatar_ros/set_mode]");
}

void AvatarConnector::expression(const string& face, const string& duration) throw (expression::exception::IOException) {
	silbot3_msgs::AvatarRos_Expression_Srv srv;
	srv.request.opname = face;
	srv.request.duartion = ofUtil::ofToInt(duration);
	srv.request.weight = 1.0;

	ROS_INFO("wating for service avaliable[/avatar_ros/expression]");
	bool success = ros::service::waitForService("/avatar_ros/expression");
	if(!success) {
		ROS_ERROR("service is not availabe..[/avatar_ros/expression]");
	} else {
		ROS_DEBUG("service is avaliable...[/avatar_ros/expression]");
	}

	if(!this->expressionClient.call(srv)) {
		expression::exception::IOException e;
		e.setMessage("IOException : cannot call service [/avatar_ros/expression]");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
	ROS_INFO("success call service [//avatar_ros/expression]");
}


void AvatarConnector::playLipsync( const string& opname, const string& dirpath, const string& filename, const vector<silbot3_msgs::TTSViseme>& visemes ) throw (expression::exception::IOException)
{
	silbot3_msgs::AvatarRos_PlayLipSync_Srv srv;
	srv.request.opname = opname;
	srv.request.dirPath = dirpath;
	srv.request.filename = filename;
	srv.request.visemes = visemes;


	ROS_INFO("wav dir path : %s", dirpath.c_str());
	ROS_INFO("wav file name : %s", filename.c_str());

	ROS_INFO("wating for service will be available..........play_lip_sync");
	bool success = ros::service::waitForService("/avatar_ros/play_lip_sync");
	if(!success) {
		ROS_ERROR("play_lip_sync service is not available yet!!");
	} else {
		ROS_INFO("play_lip_sync service available..");
	}

	if(playLipsyncSrv.call(srv)){
		ROS_INFO("%s", srv.response.result.c_str());

	}else{
		expression::exception::IOException e;
		e.setMark(__FILE__, __LINE__);
		e.setMessage("IOException: " + srv.response.result);
		throw e;
	}

}

void AvatarConnector::control( const string& opname ) throw (expression::exception::IOException)
{
	if(opname == "stop") {
		silbot3_msgs::AvatarRos_PlayLipSync_Srv srv;
		srv.request.opname = "stop";


		ROS_INFO("wating for service will be available..........play_lip_sync");
		bool success = ros::service::waitForService("/avatar_ros/play_lip_sync");
		if(!success) {
			ROS_ERROR("play_lip_sync service is not available yet!!");
		} else {
			ROS_INFO("play_lip_sync service available..");
		}

		if(playLipsyncSrv.call(srv)) {

		} else {
			expression::exception::IOException e;
			e.setMark(__FILE__, __LINE__);
			e.setMessage("IOException : cannot call /avatar_ros/play_lip_sync");
			throw e;
		}
	}
}

}} /* namespace behavior */
