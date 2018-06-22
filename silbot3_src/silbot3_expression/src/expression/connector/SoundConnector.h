#ifndef __EXPRESSION_CONNECTOR_SOUND_CONNECTOR_H__
#define __EXPRESSION_CONNECTOR_SOUND_CONNECTOR_H__

#include <expression/lipsync/VisemeInfo.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <ros/ros.h>
#include <vector>
//#include <silbot3_msgs/ttsgensrv.h>
//#include <silbot3_msgs/ttsplaysrv.h>
//#include <silbot3_msgs/ttsvisemesrv.h>
#include <expression/exception/IOException.h>
#include <std_srvs/Empty.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <silbot3_msgs/TTSMake.h>
#include <silbot3_msgs/TTSViseme.h>
#include <silbot3_msgs/SoundPlay.h>
#include <expression/exception/Exception.h>
#include <expression/db/VisemeInfoParser.h>

using namespace boost;
using namespace std;
using namespace expression::db;

namespace expression {
namespace connector {


class SoundConnector {
private:
	ros::NodeHandle nh;

	ros::ServiceClient makeTTSWavServiceClient;
	ros::ServiceClient playServiceClient;
	ros::Publisher playPublisher;
	ros::Publisher stopPublisher;

	VisemeInfoParser visemeInfoParser;

public:
	SoundConnector();
	virtual ~SoundConnector();

	static SoundConnector* getInstance();
	void getRecordingVisem(const int& rid, string& filename, vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException);

	void makeTTSWav(const string& sentence, string& filepath, vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException);
	void playSound(bool blocking, const string& filepath) throw (expression::exception::IOException);
	void stop()  throw (expression::exception::IOException);
};

} /* namespace expression */
}
#endif /* __EXPRESSION_CONNECTOR_SOUND_CONNECTOR_H__ */
