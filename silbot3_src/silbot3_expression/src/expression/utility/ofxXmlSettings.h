#ifndef __EXPRESSION_UTILITY_XMLSETTING_H__
#define __EXPRESSION_UTILITY_XMLSETTING_H__

#include <string>
#include <vector>
#include <string.h>
#include <tinyxml.h>

using namespace std;

#define MAX_TAG_VALUE_LENGTH_IN_CHARS		1024

class ofxXmlSettings{

	public:
        ofxXmlSettings();
        ~ofxXmlSettings();

		void setVerbose(bool _verbose);

		bool loadFile(const string& xmlFile);	//this is not relative to your data/ path - use ofDataPath(...) to make it relative
		void saveFile(const string& xmlFile);   //this is not relative to your data/ path - use ofDataPath(...) to make it relative

		void clearTagContents(const string& tag, int which = 0);
		void removeTag(const string& tag, int which = 0);

		bool tagExists(const string& tag, int which = 0);

		// removes all tags from within either the whole document
		// or the tag you are currently at using pushTag
		void	clear();

		int 	getValue(const string&  tag, int            defaultValue, int which = 0);
		double 	getValue(const string&  tag, double         defaultValue, int which = 0);
		string 	getValue(const string&  tag, const string& 	defaultValue, int which = 0);

		int 	setValue(const string&  tag, int            value, int which = 0);
		int 	setValue(const string&  tag, double         value, int which = 0);
		int 	setValue(const string&  tag, const string& 	value, int which = 0);


		bool	pushTag(const string&  tag, int which = 0);
		int		popTag();
		int		getPushLevel();

		int		getNumTags(const string& tag);

		int 	addValue(const string&  tag, int            value);
		int 	addValue(const string&  tag, double         value);
		int 	addValue(const string&  tag, const string& 	value);

		int		addTag(const string& tag); //adds an empty tag at the current level


        // Attribute-related methods
		int		addAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		addAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		addAttribute(const string& tag, const string& attribute, int value);
		int		addAttribute(const string& tag, const string& attribute, double value);
		int		addAttribute(const string& tag, const string& attribute, const string& value);

		void	removeAttribute(const string& tag, const string& attribute, int which = 0);
		void	clearTagAttributes(const string& tag, int which = 0);

		int		getNumAttributes(const string& tag, int which = 0);

		bool	attributeExists(const string& tag, const string& attribute, int which = 0);

		bool    getAttributeNames(const string& tag, vector<string>& outNames, int which = 0);

		int		getAttribute(const string& tag, const string& attribute, int defaultValue, int which = 0);
		double	getAttribute(const string& tag, const string& attribute, double defaultValue, int which = 0);
		string	getAttribute(const string& tag, const string& attribute, const string& defaultValue, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, double value, int which = 0);
		int		setAttribute(const string& tag, const string& attribute, const string& value, int which = 0);

		int		setAttribute(const string& tag, const string& attribute, int value);
		int		setAttribute(const string& tag, const string& attribute, double value);
		int		setAttribute(const string& tag, const string& attribute, const string& value);

		void	loadFromBuffer( string buffer );
		void	copyXmlToString(string & str);

		TiXmlDocument 	doc;
		bool 			bDocLoaded;

	protected:

		TiXmlHandle     storedHandle;
		int             level;


		int 	writeTag(const string&  tag, const string& valueString, int which = 0);
		bool 	readTag(const string&  tag, TiXmlHandle& valHandle, int which = 0);	// max 1024 chars...


		int		writeAttribute(const string& tag, const string& attribute, const string& valueString, int which = 0);

        TiXmlElement* getElementForAttribute(const string& tag, int which);
        bool readIntAttribute(const string& tag, const string& attribute, int& valueString, int which);
        bool readDoubleAttribute(const string& tag, const string& attribute, double& outValue, int which);
        bool readStringAttribute(const string& tag, const string& attribute, string& outValue, int which);
};

#endif /* __EXPRESSION_UTILITY_XMLSETTING_H__ */
