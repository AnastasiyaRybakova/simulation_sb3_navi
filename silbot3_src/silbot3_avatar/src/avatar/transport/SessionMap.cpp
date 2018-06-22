/*
 * SessionMap.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: ijpark
 */

#include <ros/ros.h>
#include "SessionMap.h"

namespace avatar {
namespace transport {

SessionMap::SessionMap() {

}

SessionMap::~SessionMap() {
}

void SessionMap::setSession(const std::string& key, SessionTask* session) {
	remove(key);
	registry[key] = session;

}

const SessionTask* SessionMap::getSession(const std::string& key) const {
	Registry::const_iterator itr = registry.find(key);
	if(itr != registry.end()) {
		return itr->second;
	}
	return NULL;
}

void SessionMap::remove(const std::string& key) {
	const SessionTask* task = getSession(key);
	if(task != NULL) {
		ROS_DEBUG("session[%s] destructed", task->getName().c_str());
		registry.erase(key);
	}
}

int SessionMap::size() const {
	//return registry.size();
	return registry.size();
}

vector<string> SessionMap::keySet() const {
	vector<string> keys;
	Registry::const_iterator itr;
	for(itr = registry.begin(); itr != registry.end(); itr++) {
		keys.push_back(itr->first);
	}

	return keys;
}

SessionMap* SessionMap::getInstance() {
	static SessionMap instance;
	return &instance;
}

} /* namespace transport */
} /* namespace avatar */
