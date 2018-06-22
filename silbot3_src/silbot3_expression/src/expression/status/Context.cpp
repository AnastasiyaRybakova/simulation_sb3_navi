#include "Context.h"

	Context::Context() {
		publisher = StatusPublisher::getInstance();
		currentStatus = EXPRESSION_STATUS_INIT;
		expressionWithMotionPlayer.setstatusNotificationListener(this);
		expressionWithoutMotionPlayer.setstatusNotificationListener(this);
		expressionWithSongPlayer.setstatusNotificationListener(this);
		this->currentPlayType = EXPRESSION_STATUS_INIT;
		 //a_mutex = PTHREAD_MUTEX_INITIALIZER;
	}

	Context::~Context() {

	}

	void Context::moveToInit() {

		if(currentPlayType == EXPRESSION_STATUS_WITH_MOTION) {
			this->expressionWithMotionPlayer.stopPlay();
		} else if(currentPlayType == EXPRESSION_STATUS_WITHOUT_MOTION) {
			this->expressionWithoutMotionPlayer.stopPlay();
		} else if(currentPlayType == EXPRESSION_STATUS_WITH_SONG) {
			this->expressionWithSongPlayer.stopPlay();
		}
		this->currentPlayType = EXPRESSION_STATUS_INIT;
		this->currentStatus = EXPRESSION_STATUS_INIT;
	}

	void Context::moveToExpressionWithMotion(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException) {
		try {
			if(currentStatus != EXPRESSION_STATUS_INIT) {
				expression::exception::CannotChangeStatusException e;
				e.setMessage("Cannot change state because of playing prevalent behavior");
				e.setMark(__FILE__, __LINE__);
				throw e;
			}

			currentStatus =  EXPRESSION_STATUS_WITH_MOTION;
			this->expressionWithMotionPlayer.setParameter(parameter);
			this->expressionWithMotionPlayer.setModalityUsetSet(modality);
			this->expressionWithMotionPlayer.startPlay();
			this->currentPlayType = EXPRESSION_STATUS_WITH_MOTION;
		} catch(expression::exception::CannotChangeStatusException& e) {
			std::stringstream ss;
			ss<<"CannotChangeStatusException ["<<currentStatus<<" to "<<EXPRESSION_STATUS_WITH_MOTION<<"]";
			e.setMessage(ss.str());
			e.setMark(__FILE__, __LINE__);
			throw e;
		} catch (expression::exception::CannotPlayException& e) {
			expression::exception::CannotChangeStatusException ee;
			ee.setMessage(e.getMessage());
			ee.setStackTrace(e.getStackTrace());
			ee.setMark(__FILE__, __LINE__);
			currentStatus = EXPRESSION_STATUS_INIT;
			throw ee;
		}

	}

	void Context::moveToExpressionWithoutMotion(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException) {
		//pthread_mutex_lock(&a_mutex);
		//boost::shared_lock<boost::shared_mutex> lock(_access);
		//boost::lock_guard<boost::mutex> lock(_access);
		//boost::mutex::scoped_lock scoped_lock(stateLock);
		try {
			if(currentStatus != EXPRESSION_STATUS_INIT) {
				expression::exception::CannotChangeStatusException e;
				std::stringstream ss;
				ss<<"CannotChangeStatusException ["<<currentStatus<<" to "<<EXPRESSION_STATUS_WITHOUT_MOTION<<"]";
				e.setMessage(ss.str());
				e.setMark(__FILE__, __LINE__);
				throw e;
			}

			//canChangeState(EXPRESSION_STATUS_WITHOUT_MOTION);

			currentStatus =  EXPRESSION_STATUS_WITHOUT_MOTION;
			this->expressionWithoutMotionPlayer.setParameter(parameter);
			this->expressionWithoutMotionPlayer.setModalityUsetSet(modality);
			this->expressionWithoutMotionPlayer.startPlay();
			this->currentPlayType = EXPRESSION_STATUS_WITHOUT_MOTION;
		} catch(expression::exception::CannotChangeStatusException& e) {
			std::stringstream ss;
			ss<<"CannotChangeStatusException ["<<currentStatus<<" to "<<EXPRESSION_STATUS_WITHOUT_MOTION<<"]";
			e.setMessage(ss.str());
			e.setMark(__FILE__, __LINE__);
			throw e;
		} catch (expression::exception::CannotPlayException& e) {
			expression::exception::CannotChangeStatusException ee;
			ee.setMessage(e.getMessage());
			ee.setStackTrace(e.getStackTrace());
			ee.setMark(__FILE__, __LINE__);
			currentStatus = EXPRESSION_STATUS_INIT;
			throw ee;
		}
	}

	void Context::moveToExpressionWithSong(const ExpressionParameter& parameter, const ModalityUseSet& modality) throw (expression::exception::CannotChangeStatusException) {
		//pthread_mutex_lock(&a_mutex);
		//boost::shared_lock<boost::shared_mutex> lock(_access);
		//boost::lock_guard<boost::mutex> lock(_access);
		//boost::mutex::scoped_lock scoped_lock(stateLock);
		try {
			if(currentStatus != EXPRESSION_STATUS_INIT) {
				CannotChangeStatusException e;
				std::stringstream ss;
				ss<<"CannotChangeStatusException ["<<currentStatus<<" to "<<EXPRESSION_STATUS_WITH_SONG<<"]";
				e.setMessage(ss.str());
				e.setMark(__FILE__, __LINE__);
				throw e;
			}

			//canChangeState(EXPRESSION_STATUS_WITH_SONG);

			currentStatus =  EXPRESSION_STATUS_WITH_SONG;
			this->expressionWithSongPlayer.setParameter(parameter);
			this->expressionWithSongPlayer.setModalityUsetSet(modality);
			this->expressionWithSongPlayer.startPlay();
			this->currentPlayType = EXPRESSION_STATUS_WITH_SONG;
		} catch(expression::exception::CannotChangeStatusException& e) {
			std::stringstream ss;
			ss<<"CannotChangeStatusException ["<<currentStatus<<" to "<<EXPRESSION_STATUS_WITH_SONG<<"]";
			e.setMessage(ss.str());
			e.setMark(__FILE__, __LINE__);
			throw e;
		} catch (expression::exception::CannotPlayException& e) {
			expression::exception::CannotChangeStatusException ee;
			ee.setMessage(e.getMessage());
			ee.setStackTrace(e.getStackTrace());
			ee.setMark(__FILE__, __LINE__);
			currentStatus = EXPRESSION_STATUS_INIT;
			throw ee;
		}
	}


	void Context::handleNotification(const string& uuid, const StatusNotificationType& notification, const StatusType& notifier) {

		cout << notification << " " << notifier << endl;

		if((notification == MOTION_FINISH) &&
			((notifier == EXPRESSION_STATUS_WITH_MOTION) || (notifier == EXPRESSION_STATUS_WITHOUT_MOTION) || (notifier == EXPRESSION_STATUS_WITH_SONG))) {

			if(currentStatus == EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH) {
				currentStatus = EXPRESSION_STATUS_INIT;
			} else {
				currentStatus = EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH;
			}
			ROS_DEBUG("Contect handle notification .... 1");

		} else if((notification == START) &&
			((notifier == EXPRESSION_STATUS_WITH_MOTION) || (notifier == EXPRESSION_STATUS_WITHOUT_MOTION) || (notifier == EXPRESSION_STATUS_WITH_SONG))) {
			currentStatus  = notifier;
			ROS_DEBUG("Contect handle notification .... 3");
		} else if(notification == SOUND_FINISH) {
			if(currentStatus == EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH || currentStatus == EXPRESSION_STATUS_WITHOUT_MOTION) {
				currentStatus = EXPRESSION_STATUS_INIT;
			} else {
				currentStatus = EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH;
			}
			ROS_DEBUG("Contect handle notification .... 4");
		}
		ROS_DEBUG("Behavior handleNotification!!!");
	}

	int Context::getCurrentStatus() {
		// uint8 EXPRESSION_STATUS_INIT = 0
		// uint8 EXPRESSION_STATUS_WITH_MOTION = 1
		// uint8 EXPRESSION_STATUS_WITHOUT_MOTION = 2
		// uint8 EXPRESSION_STATUS_WITH_SONG = 3
		// uint8 EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH = 4
		// uint8 EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH = 5

		int result = -1;
		switch(currentStatus) {
		case EXPRESSION_STATUS_INIT:
			result = 0;
			break;
		case EXPRESSION_STATUS_WITH_MOTION:
			result = 1;
			break;
		case EXPRESSION_STATUS_WITHOUT_MOTION:
			result = 2;
			break;
		case EXPRESSION_STATUS_WITH_SONG:
			result = 3;
			break;
		case EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH:
			result = 4;
			break;
		case EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH:
			result = 5;
			break;
		default:
			break;
		}


				// int result = -1;
				// switch(currentStatus) {
				// case EXPRESSION_STATUS_INIT:
				// 	result = 0;
				// 	break;
				// case EXPRESSION_STATUS_WITH_MOTION:
				// 	result = 1;
				// 	break;
				// case EXPRESSION_STATUS_WITHOUT_MOTION:
				// 	result = 2;
				// 	break;
				// case EXPRESSION_STATUS_WITH_SONG:
				// 	result = 3;
				// 	break;
				// case EXPRESSION_STATUS_WAIT_FOR_MOTION_FINISH:
				// 	result = 4;
				// 	break;
				// case EXPRESSION_STATUS_WAIT_FOR_SOUND_FINISH:
				// 	result = 5;
				// 	break;
				// default:
				// 	break;
				// }



		return result;
	}
