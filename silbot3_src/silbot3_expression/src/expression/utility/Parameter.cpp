#include "Parameter.h"

using namespace behavior::util;

Parameter::Parameter(const string& name, const string& value, const string& content) {
	_name = name;
	_value = value;
	_content = content;
}

Parameter::~Parameter() {

}

string Parameter::getName() const {
	return _name;
}

string Parameter::getValue() const {
	return _value;
}

string Parameter::getContent() const {
	return _content;
}

string Parameter::toString() const {
	string parameterString = "<param";
	if(_name != "" && _name != "\0") {
		parameterString += " name=\"";
		parameterString += _name;
		parameterString += "\"";
	} else {
		return "";
	}
	if(_value != "" && _value != "\0") {
		parameterString += " value=\"";
		parameterString += _value;
		parameterString += "\"";
	} else {
		return "";
	}
	if(_content != "" && _content != "\0") {
		parameterString += "><![CDATA[";
		parameterString += _content;
		parameterString += "]]></param>";
	} else {
		parameterString += "/>";
	}
	return parameterString;
}
