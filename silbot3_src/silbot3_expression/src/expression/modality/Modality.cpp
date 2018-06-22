#include "Modality.h"

namespace expression {
	namespace modality {

		Modality::Modality(void) {
		}

		Modality::Modality( string modalityName, string functionName ): m_modalityName(modalityName), m_functionName(functionName)	{
		}

		Modality::~Modality(void) {
		}

		bool Modality::addParameter( string name, string value ) {
			m_parameter[name] = value;
			return true;
		}

	} /* namespace modality */
} /* namespace behavior */
