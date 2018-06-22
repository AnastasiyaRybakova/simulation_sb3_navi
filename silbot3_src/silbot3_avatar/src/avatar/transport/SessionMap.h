/*
 * SessionMap.h
 *
 *  Created on: Jul 16, 2013
 *      Author: ijpark
 */

#ifndef SESSIONMAP_H_
#define SESSIONMAP_H_

#include <string>
#include <map>
#include "SessionTask.h"

typedef std::map<std::string, avatar::transport::SessionTask*> Registry;

namespace avatar {
namespace transport {

class SessionMap {
private:
	Registry registry;

public:
	SessionMap();
	virtual ~SessionMap();

public:
	void setSession(const std::string& key, avatar::transport::SessionTask* session);
	void remove(const std::string& key);
	const avatar::transport::SessionTask* getSession(const std::string& key) const;
	int size() const;

	vector<string> keySet() const;

	static SessionMap* getInstance();
};

} /* namespace transport */
} /* namespace avatar */
#endif /* SESSIONMAP_H_ */
