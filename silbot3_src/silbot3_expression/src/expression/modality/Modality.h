#ifndef __EXPRESSION_MODALITY_MODALITY_H__
#define __EXPRESSION_MODALITY_MODALITY_H__

#include <string>
#include <map>

using namespace std;

namespace expression {
	namespace modality {

		class Modality 	{

		public:
			Modality(void);
			~Modality(void);

			Modality(string modalityName, string functionName);

			std::string ModalityName() const { return m_modalityName; }
			void ModalityName(std::string val) { m_modalityName = val; }

			std::string FunctionName() const { return m_functionName; }
			void FunctionName(std::string val) { m_functionName = val; }

			bool addParameter( string name, string value);

			map<string, string> m_parameter;

		private:
			string m_modalityName;
			string m_functionName;
		};

	} /* namespace modality */
} /* namespace expression */

#endif /* __EXPRESSION_MODALITY_MODALITY_H__ */
