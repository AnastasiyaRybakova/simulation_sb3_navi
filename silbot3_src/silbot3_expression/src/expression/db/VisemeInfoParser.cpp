/*
 * VisemeInfoParser.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: ijpark
 */

#include "VisemeInfoParser.h"
#include <ros/ros.h>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include <sys/stat.h>
#include <time.h>

using namespace boost;

namespace expression {
	namespace db {

		VisemeInfoParser::VisemeInfoParser() {

		}

		VisemeInfoParser::~VisemeInfoParser() {
		}


		void VisemeInfoParser::parse()  {
			stringstream filepath;
			filepath << "/opt/robocare/common/sound/recorded/RecordedVisemeInfo.xml";
			string xml = filepath.str();
			if(!boost::filesystem::exists(xml)) {
				stringstream ss;
				ss<<"NosuchFileException : ["<<xml<<"]";
				Exception e;
				e.setMessage(ss.str());
				e.setMark(__FILE__, __LINE__);
				throw e;
			}

			ROS_INFO("viseme file [ %s]", xml.c_str());

			bool loadSuccess = document.LoadFile(xml);

			TiXmlElement* rootelement = document.FirstChildElement("sentences");
			TiXmlElement* sentenceEl = rootelement->FirstChildElement("sentence");

			TiXmlElement* el = NULL;
			int count = 0;
			for(el = sentenceEl; el != NULL; el = el->NextSiblingElement("sentence")) {
				count++;
				RecordedVisemeInfo viseme;
				string sid = el->FirstChildElement("id")->GetText();
				string text = el->FirstChildElement("text")->GetText();
				string sviseme = el->FirstChildElement("viseme")->GetText();
				string wavName = el->FirstChildElement("wavName")->GetText();

				vector<Viseme> tempVis;
				split(sviseme, tempVis);

				viseme.recordID =  boost::lexical_cast<int>(sid);
				viseme.text =  text;
				viseme.visemes = sviseme;
				viseme.wavfilename = wavName;
				viseme.visemeInfos = tempVis;
				visemeMap[viseme.recordID] = viseme;
			}
			ROS_INFO("viseme file parse success count : %d ", count);
		}

		const VisemeList& VisemeInfoParser::getVisemeList() const {
			return visemeList;
		}

		const RecordedVisemeInfo& VisemeInfoParser::getVisemeInfo(const int& id) const throw (expression::exception::ArrayOutofBoundsException) {
			std::stringstream sb;
			if(id >= (int)visemeList.size() + 1) {
				sb<<"ArrayOutofBoundsException : "<<id<<" >="<<visemeList.size() + 1;
				expression::exception::ArrayOutofBoundsException e;
				e.setMark(__FILE__, __LINE__);
				e.setMessage(sb.str());
				throw e;
			}

			if(id <= 1) {
				sb<<"ArrayOutofBoundsException : "<<id<<" <=1";
				expression::exception::ArrayOutofBoundsException e;
				e.setMark(__FILE__, __LINE__);
				e.setMessage(sb.str());
				throw e;
			}

			return visemeList[id];
		}


		const RecordedVisemeInfo& VisemeInfoParser::getViseme(const int& rid /*recordedsentenceid*/)  const throw (expression::exception::NoSuchElementException){
			VisemeInfoMap::const_iterator itr = visemeMap.find(rid);
			if(itr == visemeMap.end()) {
				expression::exception::NoSuchElementException e;
				std::stringstream ss;
				ss<<"NosuchElementException : ["<<rid<<"]";
				e.setMessage(ss.str());
				e.setMark(__FILE__, __LINE__);
				throw e;
			} else {
				cout << "adfsdafasdfdsfds"<< endl;
				return itr->second;
			}
		}

		void VisemeInfoParser::split( string strVisemes, vector<Viseme>& result )
		{
			string text = strVisemes;
			char_separator<char> sep("|");
			char_separator<char> sep2(",");
			tokenizer<char_separator<char> > tokens(text, sep);
			tokenizer<char_separator<char> >::const_iterator itr;


			int index = 0;

			for(itr = tokens.begin(); itr != tokens.end(); itr++) {
				Viseme v;
				tokenizer<char_separator<char> > tokens2(*itr, sep2);
				tokenizer<char_separator<char> >::const_iterator itr2 = tokens2.begin();
				v.visemeID = atoi(((string)(*itr2)).c_str());
				itr2++;
				v.duration = atoi(((string)(*itr2)).c_str());
				index++;
				result.push_back(v);
			}

			vector<Viseme> tempv = result;

			for(unsigned int i= 0; i<tempv.size(); i++){
				result[i].duration =tempv[i+1].duration - tempv[i].duration;
			}

			result[result.size() - 1].duration = 50;

		//	cout <<"Viseme Info: ";
		//	for(int i = 0; i< result.size(); i++){
		//		cout << result[i].visemeID << ","<< result[i].duration << "|";
		//	}
		//	cout <<endl;

		}
		string  VisemeInfoParser::intToString(int intNumber)
		{
			stringstream ssID;
			ssID << intNumber;
			return ssID.str();
		}
	}
} /* namespace tts */
