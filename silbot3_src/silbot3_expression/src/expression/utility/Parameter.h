#ifndef PARAMETER_
#define PARAMETER_

#include <boost/shared_ptr.hpp>
#include <string>

using namespace std;

namespace behavior {
	namespace util {
		class Parameter {

		private:
			string _name;
			string _value;
			string _content;

		public:
			Parameter(const string& name, const string& value, const string& content);
			virtual ~Parameter();

		public:
			string getName() const;
			string getValue() const;
			string getContent() const;
			string toString() const;

			void setName(const string& name);
			void setValue(const string& value);
			void setContent(const string& content);
		};

		typedef boost::shared_ptr<Parameter> sParameterPtr;

	}
}

#endif /*PARAMETER_*/
