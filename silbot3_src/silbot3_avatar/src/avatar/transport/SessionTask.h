/*
 * SessionTask.h
 *
 *  Created on: 2013. 7. 12.
 *      Author: ijpark
 */

#ifndef SESSIONTASK_H_
#define SESSIONTASK_H_


#include <string>
//#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <avatar/connector/WSocket.h>
#include <avatar/message/GameMessage.h>

using namespace avatar::message;


namespace avatar {
namespace transport {


class SessionTask {
public:
	SessionTask();
	SessionTask(WSocket* socket);
	virtual ~SessionTask();

	void run();
	void start();
//	void threadEntry();
//	void threadEntry(boost::thread* thread, tcp::socket* socket);
	void stop();
	//void setListener(SessionTaskListener* listener);

	const boost::thread* getThread() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	void broadcast(GameMessage& message);
	void writeMessage(GameMessage& message);


private:
	bool runningFlag;

	//tcp::socket* socket;
	WSocket* socket;
	boost::thread thread;
	std::string name;

};

} /* namespace transport */
} /* namespace avatar */
#endif /* SESSIONTASK_H_ */
