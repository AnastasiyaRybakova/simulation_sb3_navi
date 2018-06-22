/*
 * MessageReceiver.h
 *
 *  Created on: Jul 16, 2013
 *      Author: ijpark
 */

#ifndef MESSAGERECEIVER_H_
#define MESSAGERECEIVER_H_

#include <string>
#include <sstream>
//#include <boost/asio.hpp>
#include <avatar/connector/WSocket.h>
#include <avatar/message/GameMessage.h>
#include <robocare/exception/IOException.h>


using namespace std;
using namespace roscir::exception;

namespace avatar {
namespace transport {

class MessageReceiver {
private:
	//tcp::socket* socket;
	WSocket* socket;
public:
	MessageReceiver(WSocket* socket);
	virtual ~MessageReceiver();

	int read(const int& bufsize, unsigned char* buffer) throw (IOException);
	int readXML(string& buffer) throw (IOException);
	int readLine(string& buffer) throw (IOException);
	void getMessage(avatar::message::GameMessage& message) throw (IOException);
};

} /* namespace transport */
} /* namespace avatar */
#endif /* MESSAGERECEIVER_H_ */
