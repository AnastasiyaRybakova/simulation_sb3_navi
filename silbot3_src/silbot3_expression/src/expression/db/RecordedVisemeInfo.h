
#ifndef RECORDEDVESEMEINFO_H_
#define RECORDEDVESEMEINFO_H_

#include <string>
#include <vector>

using namespace std;
namespace expression {
	namespace db {

		class Viseme {
		public:
			int visemeID;
			int duration;
		};

		class RecordedVisemeInfo {
		public:
			int recordID;
			string text;
			string visemes;
			string wavfilename;
			vector<Viseme> visemeInfos;


		public:
			RecordedVisemeInfo();
			virtual ~RecordedVisemeInfo();

			const int& getId() const;
			const string& getText() const;
			const string& getViseme() const;
			const string& getWavname() const;

			string toString() const;

		};
	}
} /* namespace lipsync */
#endif /* RECORDEDVESEMEINFO_H_ */
