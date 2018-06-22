#include "ExpressionDB.h"

namespace expression {
namespace db {

ExpressionDB::ExpressionDB() {
}

ExpressionDB::~ExpressionDB() {
}

void ExpressionDB::setSoundSource(const std::string& path) {
	this->soundSource = path;
}


void ExpressionDB::setExpressionSequenceList(const std::vector<ExpressionSequence>& sequnces) {
	this->expressionSequenceList = sequnces;
}

const std::vector<ExpressionSequence>& ExpressionDB::getExpressionSequenceList() const {
	return this->expressionSequenceList;
}

} /* namespace db */
} /* namespace behavior */
