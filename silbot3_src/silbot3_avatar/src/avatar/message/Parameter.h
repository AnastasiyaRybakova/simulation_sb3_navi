#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <iostream>
using namespace std;

namespace avatar {
	namespace message{
		class Parameter {

		private :
			string name;
			string values;

		public :
			Parameter(void);
			Parameter(const string& iName, const string& iValue);
			~Parameter(void);

		public :
			string getName();
			string getValue();
		};
	}}

#endif
