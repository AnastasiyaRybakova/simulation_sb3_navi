#ifndef __SILBOT3_EXPRESSION_H__
#define __SILBOT3_EXPRESSION_H__

#include <vector>

#include <ros/ros.h>
#include <boost/thread/mutex.hpp>

#include <expression/modality/Modality.h>
#include <expression/modality/ModalityUseSet.h>
#include <expression/protocol/ExpressionParameter.h>
#include <expression/db/ExpressionDB.h>
#include <expression/utility/ofLog.h>

#include <expression/connector/DeviceConnector.h>
#include <expression/connector/AvatarConnector.h>
#include <expression/connector/SoundConnector.h>

using namespace std;
using namespace expression;
using namespace expression::modality;
using namespace expression::parameter;
using namespace expression::db;
using namespace expression::connector;

namespace expression {

class Expression {


private:
	DeviceConnector* _device_conn;

	bool runningFlag;
	boost::mutex stateLock;

	int delayType;

	boost::thread pushThread;
	boost::thread homingThread;


public:
	Expression();
	virtual ~Expression();

	void playWrongMessageSound();
	void setModality(const expression::modality::ModalityUseSet& modality);
	bool moveToHome(const ExpressionParameter& parameter, const ModalityUseSet& modality);
	void executeDB(const ExpressionParameter& BehaviorInformation, const ModalityUseSet& modality, ExpressionDB& db);
	void executeModality(const ExpressionParameter& parameter, const ModalityUseSet& modalityUseSet,  Modality &mod );
	bool mpExpressEmotion(string emo, string duration, string weight );
	bool mpExpress( int duration, int facenum, float weight );
	bool mpExpressLookAt( string duration, string x, string y , string weight);
	void wheelStop();

	void setExressionFlag(bool flag);
	bool getRunningFlag();
};

} /* namespace expression */
#endif /* __SILBOT3_EXPRESSION_H__ */
