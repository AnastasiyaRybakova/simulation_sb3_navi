/*
 * UUID.h
 *
 *  Created on: 2013. 5. 22.
 *      Author: root
 */

#ifndef UUID_H_
#define UUID_H_

#include <string>

namespace avatar {
namespace util {

class UUID {
public:
	UUID();
	virtual ~UUID();

	static std::string genRandom();
};

}}


#endif /* UUID_H_ */
