#ifndef __EXPRESSION_PLAYER_H__
#define __EXPRESSION_PLAYER_H__

#include <expression/modality/ModalityUseSet.h>
#include <expression/status/StatusNotificationListener.h>

using namespace expression::status;
using namespace expression::notification;
using namespace expression::modality;

namespace expression {
namespace player {

class ExpressionPlayer {
protected:
	ModalityUseSet modalityUseSet;
	StatusNotificationListener* notificationListener;

public:
	ExpressionPlayer();
	virtual ~ExpressionPlayer();

	virtual void startPlay() = 0;
	virtual void stopPlay() = 0;
	virtual void setModalityUsetSet(const ModalityUseSet& set);
	void setNotificationListener(StatusNotificationListener* handler);

};

} /* namespace player */
} /* namespace expression */
#endif /* __EXPRESSION_PLAYER_H__ */
