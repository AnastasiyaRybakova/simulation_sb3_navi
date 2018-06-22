#ifndef __EXPRESSION_DB_LOADER_H__
#define __EXPRESSION_DB_LOADER_H__


#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include <expression/db/ExpressionDB.h>
#include <expression/db/ExpressionSequence.h>
#include <expression/protocol/ExpressionParameter.h>

#include <expression/utility/ofxXmlSettings.h>
#include <expression/utility/ofUtil.h>

#include <robocare/exception/FileNotFoundException.h>
#include <robocare/exception/ParseException.h>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace expression;
using namespace expression::modality;
using namespace expression::db;
using namespace expression::parameter;

namespace expression {
	namespace db {

		class ExpressionDBLoader {

		private:
			std::vector<ExpressionSequence> expressionSequenceList;
			ofxXmlSettings XML;
			string _songDir;
			string _expressionSoundDir;
			string _songName;
			double _songTime;

		public:
			ExpressionDBLoader();
			~ExpressionDBLoader();

			bool loadExpressionDBFromFile( string filename , ExpressionDB& db);
			bool DBLoaded;
			static std::string makeFilenameFromExpressionInfo( const ST_Expression& exp, bool isXML = true );
			string getSongDir();
			string getSongName();
			double getSongTime();

		private:
			void loadExpressionSequence(ExpressionDB& db, int id , double offsetTime = 0 );
			void modityStartTimeEndTime();
			bool findSoundModality();
			bool getSoundModality(Modality & mod);
			bool loadSoundWav( ExpressionSequence &bs, ExpressionDB& db );
			bool loadPlayTTS( ExpressionSequence &bs );

			int findSpeakModality();
			void setSpeakTime();

			void removeSpeakModalities( int count );

			bool loadEngkeyAvatarExpress( ExpressionSequence &bs );
			bool loadEngkeyAvatarPlaySongLipsync( ExpressionSequence &bs );
			bool loadEngkeyAvatarMpExpressEmotion( ExpressionSequence &bs, string emo );
			bool loadEngkeyAvatarLookAt( ExpressionSequence &bs );
			bool loadEngkeyAvatarCloseEye( ExpressionSequence &bs );
			bool loadEngkeyAvatarBlink( ExpressionSequence &bs );
			bool loadEngkeyAvatarPupilPos( ExpressionSequence &bs );
			bool loadEngkeyAvatarSetEyeClose( ExpressionSequence &bs );

			bool loadAvatarFaceParam( ExpressionSequence &bs );
			bool loadAvatarPlayMocap( ExpressionSequence &bs );

			bool loadEngkeyKiboNeck( ExpressionSequence &bs );
			bool loadERobotNeck( ExpressionSequence &bs );
			bool loadEngkeyFace( ExpressionSequence &bs );

			bool loadLedExpression( ExpressionSequence &bs );

			bool loadMoveByWheelVelocity( ExpressionSequence &bs );
			bool loadMoveByVelocityXYT( ExpressionSequence &bs );
			bool loadMoveToDeltaXYTheta( ExpressionSequence &bs );
			bool loadPosition( ExpressionSequence &bs );

			bool loadEngkeyArm( ExpressionSequence &bs , string ms);
			bool engkeyArm2DOF( int numParameterTags, ExpressionSequence &bs );
			bool engkeyArm3DOF( int numParameterTags, ExpressionSequence &bs );
			bool eRobotArm3DOF( int numParameterTags, ExpressionSequence &bs );

		};

	} /* namespace db */
} /* namespace expression */

#endif /* __EXPRESSION_DB_LOADER_H__ */
