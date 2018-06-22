#ifndef __EXPRESSION_WITHOUT_MOTION_PLAYER_H__
#define __EXPRESSION_WITHOUT_MOTION_PLAYER_H__

#include <vector>
#include <ros/ros.h>
#include <boost/thread.hpp>

#include <expression/player/ExpressionPlayer.h>
#include <expression/Expression.h>
#include <expression/exception/CannotPlayException.h>
#include <expression/status/Status.h>
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

class ExpressionWithoutMotionPlayer  {

private :
	bool speakNow;
	ExpressionParameter parameter;
	ModalityUseSet modality;
	ExpressionDB db;

	StatusNotificationListener* statusNotificationListener;
	Lipsync* lipsync;

	boost::thread speakThread;

public:
	ExpressionWithoutMotionPlayer();
	virtual ~ExpressionWithoutMotionPlayer();

	void startPlay() throw (CannotPlayException);
	void stopPlay();
	void playLipsyncThread();

	void setParameter(const ExpressionParameter& parameter);
	void setModalityUsetSet(const ModalityUseSet& modalityUsetSet);

	void setstatusNotificationListener(StatusNotificationListener* listener);
};

} /* namespace player */
} /* namespace expression */
#endif /* __EXPRESSION_WITHOUT_MOTION_PLAYER_H__ */
