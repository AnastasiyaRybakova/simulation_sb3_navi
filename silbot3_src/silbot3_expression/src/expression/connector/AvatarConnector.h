/*
 * AvatarConnector.h
 *
 *  Created on: 2014. 7. 9.
 *      Author: ADmin
 */

#ifndef __EXPRESSION_CONNECTOR_AVATARCONNECTOR_H_
#define __EXPRESSION_CONNECTOR_AVATARCONNECTOR_H_

#include <string>

#include <ros/ros.h>
#include <boost/thread.hpp>

#include <expression/exception/IOException.h>

#include <silbot3_msgs/AvatarRos_PlayMocapSong_Srv.h>
#include <silbot3_msgs/AvatarRos_SetMode_Srv.h>
#include <silbot3_msgs/AvatarRos_Expression_Srv.h>
#include <silbot3_msgs/AvatarRos_PlayLipSync_Srv.h>
#include <silbot3_msgs/TTSViseme.h>
#include <expression/utility/ofUtil.h>

using namespace std;

namespace expression {
	namespace connector {
		class AvatarConnector {
		private:
			ros::NodeHandle nh;
			ros::ServiceClient playMocapClient;
			ros::ServiceClient setModeClient;
			ros::ServiceClient expressionClient;
			ros::ServiceClient playLipsyncSrv;
		public:
			AvatarConnector();
			virtual ~AvatarConnector();

			static AvatarConnector* getInstance();

			void playMocap(const string& songDir, const string& songFile,  const string& mocapDir, const string& mocapFile) throw (expression::exception::IOException);
			void setMode(const string& mode) throw (expression::exception::IOException);
			void expression(const string& face, const string& duration) throw (expression::exception::IOException);
			void playLipsync(const string& opname, const string& dirpath, const string& filename, const vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException);
			void control(const string& opname) throw (expression::exception::IOException);

		};

	}
} /* namespace expression */


#endif /* __EXPRESSION_CONNECTOR_AVATARCONNECTOR_H_ */
