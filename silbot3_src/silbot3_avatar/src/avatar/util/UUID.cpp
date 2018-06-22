/*
 * UUID.cpp
 *
 *  Created on: 2013. 5. 22.
 *      Author: root
 */

#include "UUID.h"

#include <string>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

namespace avatar {
namespace util {

UUID::UUID() {
}

UUID::~UUID() {
}

string UUID::genRandom() {

	boost::uuids::random_generator gen;
	boost::uuids::uuid u = gen();
	const std::string tmp = boost::lexical_cast<std::string>(u);
	return tmp;
}

}} /* namespace tts */
