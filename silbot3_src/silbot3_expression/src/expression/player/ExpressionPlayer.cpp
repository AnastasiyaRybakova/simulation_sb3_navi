#include "ExpressionPlayer.h"

namespace expression {
namespace player {

	ExpressionPlayer::ExpressionPlayer() {
	}

	ExpressionPlayer::~ExpressionPlayer() {
	}

	void ExpressionPlayer::setModalityUsetSet(const ModalityUseSet& set) {
		this->modalityUseSet = set;
	}

	void ExpressionPlayer::setNotificationListener(StatusNotificationListener* listener) {
		this->notificationListener = listener;
	}

} /* namespace player */
} /* namespace expression */
