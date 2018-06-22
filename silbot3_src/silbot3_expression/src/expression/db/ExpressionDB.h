#ifndef __EXPRESSION_DB_H__
#define __EXPRESSION_DB_H__

#include <expression/db/ExpressionSequence.h>

using namespace std;
using namespace expression::db;

namespace expression  {
	namespace db {

		class ExpressionDB {

		private:
			std::string soundSource;
			std::vector<ExpressionSequence> expressionSequenceList;

		public:
			ExpressionDB();
			virtual ~ExpressionDB();

			void setSoundSource(const std::string& path);
			void setExpressionSequenceList(const std::vector<ExpressionSequence>& sequnces);
			const std::vector<ExpressionSequence>& getExpressionSequenceList() const;
		};
	} /* namespace db */
} /* namespace expression */

#endif /* __EXPRESSION_DB_H__ */
