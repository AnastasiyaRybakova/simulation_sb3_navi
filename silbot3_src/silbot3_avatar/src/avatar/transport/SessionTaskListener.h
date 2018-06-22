#ifndef __SESSION_TASK_LISTENER_H__
#define  __SESSION_TASK_LISTENER_H__

#include <avatar/transport/SessionTask.h>

namespace avatar {
namespace transport {

class SessionTaskListener {
public:
	virtual ~SessionTaskListener(){}
	virtual void sessionStart(avatar::transport::SessionTask* task) = 0;
	virtual void sessionFinish(avatar::transport::SessionTask* task) = 0;
};

}}

#endif
