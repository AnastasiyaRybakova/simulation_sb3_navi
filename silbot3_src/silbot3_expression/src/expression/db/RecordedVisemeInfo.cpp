/*
 * VesemeInfo.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: ijpark
 */

#include "RecordedVisemeInfo.h"
#include <sstream>

using namespace std;

namespace expression {
	namespace db {

		RecordedVisemeInfo::RecordedVisemeInfo() {
		}

		RecordedVisemeInfo::~RecordedVisemeInfo() {
		}

		string RecordedVisemeInfo::toString() const {
				stringstream ss;
				ss<<"recordID["<<recordID<<"] text["<<text<<"] visemes["<<visemes<<"] wavFilename["<<wavfilename<<"]"<<endl;
				return ss.str();
		}

		const int& RecordedVisemeInfo::getId() const {
			return recordID;
		}


		const string& RecordedVisemeInfo::getText() const {
			return text;
		}

		const string& RecordedVisemeInfo::getViseme() const {
			return visemes;
		}

		const string& RecordedVisemeInfo::getWavname() const {
			return wavfilename;
		}
	}
} /* namespace expression */
