/*
 * MessageWriter.h
 *
 *  Created on: Jul 18, 2013
 *      Author: ijpark
 */

#ifndef MESSAGEWRITER_H_
#define MESSAGEWRITER_H_

#include <string>
#include <sstream>
#include <avatar/message/GameMessage.h>
#include <robocare/exception/IOException.h>
#include <avatar/connector/WSocket.h>


using namespace std;
using namespace avatar::message;
using namespace roscir::exception;

namespace avatar {
namespace transport {

class MessageWriter {
private:
	WSocket* socket;
public:
	MessageWriter();
	MessageWriter(WSocket* socket);
	virtual ~MessageWriter();

	void sendMessage(GameMessage& message) throw (IOException);
};

} /* namespace connector */
} /* namespace avatar */
#endif /* MESSAGEWRITER_H_ */
