#include "Parameter.h"

namespace avatar {
	namespace message{

Parameter::Parameter()
{
	
}

Parameter::~Parameter()
{

}

Parameter::Parameter(const string& iName, const string& iValue)
{
	this->name = iName;
	this->values = iValue;	
}

string Parameter::getName()
{	
	return this->name;
}

string Parameter::getValue()
{
	return this->values;
}

}}
