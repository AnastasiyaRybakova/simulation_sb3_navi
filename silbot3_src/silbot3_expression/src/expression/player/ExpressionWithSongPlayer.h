#ifndef __EXPRESSION_WITH_SONG_PLAYER_H__
#define __EXPRESSION_WITH_SONG_PLAYER_H__

#include <ros/ros.h>
#include <boost/thread.hpp>

#include <expression/Expression.h>
#include <expression/db/ExpressionDB.h>
#include <expression/db/ExpressionDBLoader.h>
#include <expression/player/ExpressionPlayer.h>
#include <expression/protocol/ExpressionParameter.h>
#include <expression/status/StatusNotificationListener.h>
#include <expression/exception/CannotPlayException.h>
#include <expression/lipsync/Lipsync.h>

#include <robocare/exception/IOException.h>

using namespace std;
using namespace expression::player;
using namespace expression::db;
using namespace expression::parameter;
using namespace expression::status;
using namespace expression::exception;
using namespace expression::connector;
using namespace expression::lipsync;

namespace expression {
	namespace player {

		class ExpressionWithSongPlayer : public ExpressionPlayer {

		private:
			ExpressionDBLoader dbLoader;
			ExpressionDB db;
			bool expressNow;
			bool songNow;
			ExpressionParameter parameter;
			ModalityUseSet modality;
			string _songName;
			string _songDIr;

			Expression expression;

			boost::thread behaviorThread;
			boost::thread songThread;

			StatusNotificationListener* statusNotificationListener;
			Lipsync* lipsync;

		private :
			void threadFunction();

		public:
			ExpressionWithSongPlayer();
			virtual ~ExpressionWithSongPlayer();

			void startPlay() throw (CannotPlayException);
			void stopPlay();

			void playExpressionThread();
			void playSongThread();

			void setParameter(const ExpressionParameter& parameter);
			void setModalityUsetSet(const ModalityUseSet& set);
			void setstatusNotificationListener(StatusNotificationListener* listener);
		};

	} /* namespace player */
} /* namespace expression */


#endif /* __EXPRESSION_WITH_SONG_PLAYER_H__ */
