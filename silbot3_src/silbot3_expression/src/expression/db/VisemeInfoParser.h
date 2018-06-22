/*
 * VisemeInfoParser.h
 *
 *  Created on: Jul 24, 2013
 *      Author: ijpark
 */

#ifndef VISEMEINFOPARSER_H_
#define VISEMEINFOPARSER_H_

#include <expression/exception/ArrayOutofBoundsException.h>
#include <expression/exception/NoSuchElementException.h>
#include <expression/db/RecordedVisemeInfo.h>
#include <tinyxml.h>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace expression::exception;
namespace expression {
	namespace db{

		typedef std::vector<RecordedVisemeInfo> VisemeList;
		typedef std::map<int, RecordedVisemeInfo> VisemeInfoMap;
		class VisemeInfoParser {
		private:
			VisemeList visemeList;
			VisemeInfoMap visemeMap;
			TiXmlDocument document;
			string preLipInfoFileStatus;
			string lipInfoFileStatus;
			void split(string strVisemes, vector<Viseme>& result);
			string intToString(int intNumber);
		public:
			VisemeInfoParser();
			virtual ~VisemeInfoParser();

			void parse();
			const VisemeList& getVisemeList() const;
			const RecordedVisemeInfo& getVisemeInfo(const int& id) const throw (expression::exception::ArrayOutofBoundsException);

			const RecordedVisemeInfo& getViseme(const int& rid) const throw (expression::exception::NoSuchElementException);


		};
	}
} /* namespace tts */
#endif /* VISEMEINFOPARSER_H_ */
