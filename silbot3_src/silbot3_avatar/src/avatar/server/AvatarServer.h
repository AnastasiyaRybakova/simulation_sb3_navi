/*
 * AvatarServer.h
 *
 *  Created on: 2013. 7. 12.
 *      Author: ijpark
 */

#ifndef AVATARSERVER_H_
#define AVATARSERVER_H_

#include <map>
#include <ros/ros.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/pool/object_pool.hpp>
#include <avatar/transport/SessionTask.h>
#include <avatar/connector/WServerSocket.h>

#include <silbot3_msgs/AvatarRos_PlayLipSync_Srv.h>
#include <silbot3_msgs/AvatarRos_Expression_Srv.h>
#include <silbot3_msgs/AvatarRos_MoveFace_Srv.h>
#include <silbot3_msgs/AvatarRos_FaceFactor_Srv.h>
#include <silbot3_msgs/AvatarRos_SetWindow_Srv.h>
#include <silbot3_msgs/AvatarRos_PlayMocapSong_Srv.h>
#include <silbot3_msgs/AvatarRos_Dialog_Srv.h>
#include <silbot3_msgs/AvatarRos_SetMode_Srv.h>
#include <silbot3_msgs/TTSViseme.h>

using namespace avatar::transport;

namespace avatar {
namespace server {

class AvatarServer {
public:
	AvatarServer();
	virtual ~AvatarServer();

	static const int __THREAD_MAX_SIZE = 10;
private:
	bool runningFlag;
	bool playLipFlag;
	int avatarMode;
	WServerSocket serversock;

	boost::thread_group threadpool;
	boost::thread worker;
	boost::thread message_sender_thread;
	boost::thread rosserviceThread;

	std::map<std::string, SessionTask*> sessionMap;


	void run();
	void handleClient(WSocket* socket);
	void startLipmessagesender(const vector<silbot3_msgs::TTSViseme> & visemes);
	void wavPlaymessagesender(string path, string fileName);
	void lipmessagesender(const vector<silbot3_msgs::TTSViseme>& visemes);
	bool playLipSyncCB( silbot3_msgs::AvatarRos_PlayLipSync_Srv::Request &request, silbot3_msgs::AvatarRos_PlayLipSync_Srv::Response &response );
	bool expressionCB( silbot3_msgs::AvatarRos_Expression_Srv::Request &request, silbot3_msgs::AvatarRos_Expression_Srv::Response &response );
	bool moveFaceCB( silbot3_msgs::AvatarRos_MoveFace_Srv::Request &request, silbot3_msgs::AvatarRos_MoveFace_Srv::Response &response );
	bool faceFactorCB( silbot3_msgs::AvatarRos_FaceFactor_Srv::Request &request, silbot3_msgs::AvatarRos_FaceFactor_Srv::Response &response );
	bool setWindowCB( silbot3_msgs::AvatarRos_SetWindow_Srv::Request &request, silbot3_msgs::AvatarRos_SetWindow_Srv::Response &response );
	bool mocapCB( silbot3_msgs::AvatarRos_PlayMocapSong_Srv::Request &request, silbot3_msgs::AvatarRos_PlayMocapSong_Srv::Response &response );
	bool dialogCB( silbot3_msgs::AvatarRos_Dialog_Srv::Request &request, silbot3_msgs::AvatarRos_Dialog_Srv::Response &response );
	bool setModeCB( silbot3_msgs::AvatarRos_SetMode_Srv::Request &request, silbot3_msgs::AvatarRos_SetMode_Srv::Response &response );
	void broadcastVisemeMessage(int visemeID);


public:
	void start();
	void startRosService(int argc, char **argv);
	void initROSService(int argc, char **argv);
	void playRandomLipsync();
	void sessionStart(SessionTask* task);
	void sessionFinish(SessionTask* task);
	void broadcast(GameMessage& message);

	static void threadEntry(AvatarServer* parameter);

};

} /* namespace server */
} /* namespace avatar */
#endif /* AVATARSERVER_H_ */
