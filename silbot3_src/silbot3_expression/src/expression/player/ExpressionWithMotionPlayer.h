#ifndef __EXPRESSION_WITH_MOTION_PLAYER_H__
#define __EXPRESSION_WITH_MOTION_PLAYER_H__

#include <vector>
#include <ros/ros.h>
#include <boost/thread.hpp>

#include <expression/player/ExpressionPlayer.h>
#include <expression/Expression.h>
#include <expression/exception/CannotPlayException.h>
#include <expression/status/StatusNotificationListener.h>
#include <expression/db/ExpressionDB.h>
#include <expression/db/ExpressionDBLoader.h>
#include <expression/protocol/ExpressionParameter.h>
#include <expression/lipsync/Lipsync.h>
#include <expression/utility/SentenceTokenizer.h>

using namespace std;
using namespace expression::player;
using namespace expression::exception;
using namespace expression::status;
using namespace expression::db;
using namespace expression::parameter;
using namespace expression::connector;
using namespace expression::lipsync;

namespace expression {
namespace player {

class ExpressionWithMotionPlayer : public ExpressionPlayer {

private:
	ExpressionParameter parameter;
	ModalityUseSet modality;
	ExpressionDB db;
	bool expressNow;
	bool speakNow;
	bool kiboExpressNow;

	Expression expression;
	StatusNotificationListener* statusNotificationListener;
	Lipsync* lipsync;

	boost::thread behaviorPlayThread;
	boost::thread speakThread;
	boost::thread kiboBehaviorPlayThread;

public:
	ExpressionWithMotionPlayer();
	virtual ~ExpressionWithMotionPlayer();

	void startPlay() throw (CannotPlayException);
	void stopPlay();

	void setParameter(const ExpressionParameter& parameter);
	void setModalityUsetSet(const ModalityUseSet& modalityUsetSet);
	void playExpressionThread();
	void playLipsyncThread();
	void playKIBOBodyExpressionThread();

	void setstatusNotificationListener(StatusNotificationListener* listener);
	//void setParameter(const ExpressionParameter& parameter) {
};

} /* namespace player */
} /* namespace behavior */
#endif /* __EXPRESSION_WITH_MOTION_PLAYER_H__ */
