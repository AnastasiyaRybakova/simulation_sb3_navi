#ifndef __PARAMETERPART_H__
#define __PARAMETERPART_H__

#include "Parameter.h"

namespace avatar {
	namespace message{

		class ParameterPart {
		public :
			ParameterPart(void);
			~ParameterPart(void);

		public :
			Parameter getParameter();

		};
	}}

#endif
