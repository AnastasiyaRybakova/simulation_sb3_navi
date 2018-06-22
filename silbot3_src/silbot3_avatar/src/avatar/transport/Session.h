/*
 * Session.h
 *
 *  Created on: 2013. 7. 12.
 *      Author: ijpark
 */

#ifndef SESSION_H_
#define SESSION_H_

namespace avatar {
namespace transport {

class Session {
public:
	virtual ~Session(){}
	virtual void run() = 0;
};

}}


#endif /* SESSION_H_ */
