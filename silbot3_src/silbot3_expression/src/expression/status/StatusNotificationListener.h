#ifndef __EXPRESSION_STATUS_NOTIFICATION_LISTENER_H__
#define __EXPRESSION_STATUS_NOTIFICATION_LISTENER_H__

#include "Status.h"
#include <string>

using namespace expression::status;

namespace expression {
	namespace notification {

		enum StatusNotificationType {
			FINISH,
			SOUND_FINISH,
			MOTION_FINISH,
			START
		};


		class StatusNotificationListener {
		public:
			StatusNotificationListener(){}
			virtual ~StatusNotificationListener(){}

			virtual void handleNotification(const std::string& uuid, const StatusNotificationType& notification, const StatusType& notifier) = 0;

		};
	} /* namespace notification */
} /* namespace expression */

#endif /* __EXPRESSION_STATUS_NOTIFICATION_LISTENER_H__ */
