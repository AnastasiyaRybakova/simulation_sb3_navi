#ifndef __EXPRESSION_STATUS_CONTEXT_H__
#define __EXPRESSION_STATUS_CONTEXT_H__

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
//#include <pthreads/pthread.h>
#include <expression/player/ExpressionWithSongPlayer.h>
#include <expression/player/ExpressionWithMotionPlayer.h>
#include <expression/player/ExpressionWithoutMotionPlayer.h>
#include <expression/status/Status.h>
#include <expression/status/StatusNotificationListener.h>
#include <expression/exception/CannotChangeStatusException.h>
#include <expression/exception/CannotPlayException.h>
#include <expression//protocol/ExpressionParameter.h>
#include <expression/modality/Modality.h>
#include <expression/modality/ModalityUseSet.h>
#include <expression/connector/StatusPublisher.h>

using namespace expression::player;
using namespace expression::parameter;
using namespace expression::modality;
using namespace expression::connector;
using namespace expression::notification;

namespace expression {
namespace status {


	class Context : public StatusNotificationListener {

	private :
		ExpressionWithSongPlayer expressionWithSongPlayer;
		ExpressionWithMotionPlayer expressionWithMotionPlayer;
		ExpressionWithoutMotionPlayer expressionWithoutMotionPlayer;

		StatusType currentPlayType;
		StatusType currentStatus;

		StatusPublisher* publisher;

		void handleNotification(const string& uuid, const StatusNotificationType& notification, const StatusType& notifier);

		//boost::mutex _access;

		//pthread_mutex_t a_mutex;

	public:
		Context();
		virtual ~Context();

		void moveToInit();
		void moveToExpressionWithMotion(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException);
		void moveToExpressionWithoutMotion(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException);
		void moveToExpressionWithSong(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException);

		int getCurrentStatus();
	};

} /* namespace status */
} /* namespace expression */
#endif /* __EXPRESSION_STATUS_CONTEXT_H__ */
