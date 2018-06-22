#ifndef __EXPRESSION_SEQUENCE_H__
#define __EXPRESSION_SEQUENCE_H__

#include <vector>
#include <expression/modality/Modality.h>

using namespace std;
using namespace expression::modality;

namespace expression  {
	namespace db {

		class ExpressionSequence {

		public:
			double m_startTime;
			vector<Modality> m_modalityList;

			ExpressionSequence();
			virtual ~ExpressionSequence();
		};

	} /* namespace db */
} /* namespace expression */

#endif /* __EXPRESSION_SEQUENCE_H__ */
