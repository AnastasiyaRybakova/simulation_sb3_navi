#include "RecordedSentenceInfo.h"

using namespace expression::lipsync;

RecordedSentenceInfo::RecordedSentenceInfo() {

}

RecordedSentenceInfo::~RecordedSentenceInfo() {
}


const int& RecordedSentenceInfo::getId() const {
	return id;
}
const string& RecordedSentenceInfo::getText() const {
	return text;
}
const string& RecordedSentenceInfo::getViseme() const {
	return viseme;
}
const string& RecordedSentenceInfo::getWavname() const {
	return wavfilename;
}
