#include "tts_node.h"

using namespace std;
using namespace tts;

int TTSNode::m_TTSResTotal = 0;

TTSNode::TTSNode()
{
  // set default values
  //speaker_id = silbot3_msgs::TTSSetProperties::SPEAKER_ID_KOREAN_FEMALE;
  //speed = 120;
  ttsInfo.language = PTTS_KO_KR;
  ttsInfo.speaker = 0;
  ttsInfo.speed = 100;
  ttsInfo.volume = 100;
  ttsInfo.pitch = 100;
  database_location = "/opt/robocare/common/ttsDB";

  initTTS();
  initROS();
}

TTSNode::~TTSNode()
{
}

void TTSNode::initTTS()
{
  int ret = PTTS_Initialize();
  if (ret != 0)
  {
    // TODO handle exception
  }

  loadDB();
}

void TTSNode::loadDB()
{
  ROS_INFO("Database Location: %s", database_location.c_str());
  string korpath = database_location + "/KO_KR";
  string engpath = database_location + "/EN_US";
  string chipath = database_location + "/ZH_CN";

#ifdef ACA_TTS
  loadEngine("KO_KR", PTTS_KO_KR, (char*)korpath.c_str());
  loadEngine("EN_US", PTTS_EN_US, (char*)engpath.c_str());
  loadEngine("ZH_CN", PTTS_ZH_CN, (char*)chipath.c_str());
#else
  loadEngine("KO_KR", KO_KR, (char*)korpath.c_str());
  loadEngine("EN_US", EN_US, (char*)engpath.c_str());
  loadEngine("ZH_CN", ZH_CN, (char*)chipath.c_str());
#endif

}

void TTSNode::loadEngine(const char* name, int id, char* path)
{
  int result = PTTS_LoadEngine(id, path, 0);
  switch(result)
  {
    case 0 : ROS_INFO("[%s] Load: OK", name); break;
		case E_PTTS_LICENSE_KEY_NOT_FOUND    : ROS_ERROR("[%s] : LICENSE_KEY_NOT_FOUND", name);	break;
		case E_PTTS_LICENSE_DATE_EXPIRED     : ROS_ERROR("[%s] : LICENSE_DATE_EXPIRED", name); break;
		case E_PTTS_LICENSE_INVALID_SYSTEM   : ROS_ERROR("[%s] : LICENSE_INVALID_SYSTEM", name); break;
		case E_PTTS_LICENSE_INVALID_KEY      : ROS_ERROR("[%s] : LICENSE_INVALID_KEY", name); break;
		default:
			ROS_ERROR("[%s] Code : %d", name, result); break;
  }
}

void TTSNode::initROS()
{
  this->makesrv = nh.advertiseService("/silbot3_tts/make",  &TTSNode::makeCB, this);
  this->setPropertiesSub = nh.subscribe<silbot3_msgs::TTSSetProperties>("/silbot3_tts/set_properties", 1000, &TTSNode::setProertiesCB, this);
}

bool TTSNode::makeCB(silbot3_msgs::TTSMake::Request& request, silbot3_msgs::TTSMake::Response& response)
{
  // string text
  // string filepath
  // ---
  // # result constants
  // uint8 TTS_RESULT_SUCCESS = 0
  // uint8 TTS_RESULT_FAILED = 1
  //
  // uint8 tts_result
  // TTSViseme[] tts_viseme_array

  // TTSViseme
  // uint8 id
  // uint16 duration


  string text = request.text;
  string filepath = request.filepath;

  ROS_INFO("make TTS \n\ttext : %s \n\tfilepath : %s", text.c_str(), filepath.c_str());

  try {
		boost::mutex::scoped_lock lock(this->ttsMutex);

    initDropWav(filepath );
    makeTTS(text, filepath);

		lock.unlock();

	} catch(tts::Exception& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
    response.tts_result = response.TTS_RESULT_FAILED;
    return false;
	}
	response.tts_result = response.TTS_RESULT_SUCCESS;
  vector<silbot3_msgs::TTSViseme> res;
	//cout << "Visemes: ";
	for(unsigned int i = 0; i < m_svVisemes.size(); i++) {
		silbot3_msgs::TTSViseme msg;
		msg.id = m_svVisemes[i].VisemeID;
		msg.duration = m_svVisemes[i].nDur;
		res.push_back(msg);
	}

	response.tts_viseme_array = res;

	return true;
}


void  TTSNode::initDropWav(const string& filepath ) throw (Exception)
{
	this->m_svVisemes.clear();

	// if(wavfilename == "") {
	// 	stringstream ssfilename;
	// 	ssfilename<<uuid<<".wav";
  //
	// 	this->m_filename = ssfilename.str();
	// }
	// else {
	// 	this->m_filename = wavfilename;
	// }
  //
	// m_TTSResTotal = 0;
  //
	// string fullpath = m_dirpath+ "/" + m_filename;
  //
	// if(string::npos == fullpath.find(".wav")){
	// 	fullpath = fullpath+ ".wav";
	// }

	try{
		if ( (fp=fopen((char*)filepath.c_str(), "wb")) == NULL) {
			cout << "Can't make TTS Output File!" << endl;
			Exception e;
			throw e;
		}
	}catch(Exception& e){
		Exception te;
		te.setMessage(e.getMessage());
		te.setMark(__FILE__, __LINE__);
		throw e;
	}
	// Prepare Wave Header
	//WAVEFILEHEADER tmp;
	//fwrite(&tmp, 1, sizeof(WAVEFILEHEADER), fp);
}

bool TTSNode::makeWavFile(string outputFileName,  int NumData)
{
	// Write Wave Header
	long FileEnd = ftell(fp);
	fseek(fp, 0, SEEK_SET);
#ifdef ACA_TTS
	if(ttsInfo.language == PTTS_KO_KR)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == PTTS_EN_US)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == PTTS_ZH_CN)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == PTTS_RU_RU)	makeWaveHeader(fp, NumData, SAMPLE_22K, 1);
#else
	if(ttsInfo.language == 0)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == 1)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == 2)	makeWaveHeader(fp, NumData, SAMPLE_16K, 1);
	else if(ttsInfo.language  == 102)	makeWaveHeader(fp, NumData, SAMPLE_22K, 1);
	else if(ttsInfo.language  == 110)	makeWaveHeader(fp, NumData, SAMPLE_22K, 1);
#endif
	fseek(fp, FileEnd, SEEK_SET);

	// Close Output File + Unload from Memory
	fclose(fp);

	return true;
}

bool TTSNode::makeWaveHeader(FILE *fp, int DURATION, int SAMPLE_RATE, int CHANNEL)
{
  cout << "TTSNode::makeWaveHeader, numData : " << DURATION << " , rate : " << SAMPLE_RATE << " , ch : "<< CHANNEL << endl;
	// WAVEFILEHEADER	WaveFileHeader;
  //
	// strcpy(WaveFileHeader.szRiff, "RIFF");
	// strcpy(WaveFileHeader.szWave, "WAVEfmt ");
	// strcpy(WaveFileHeader.szID3, "data");
	// WaveFileHeader.nFormat          = 1;
	// WaveFileHeader.lSamplesPerSec   = SamplingRate;
	// WaveFileHeader.lUnknown         = 16;
	// WaveFileHeader.nChannels        = Ch;
	// WaveFileHeader.lBlockAlign      = 2;
	// WaveFileHeader.nBits            = 16;
	// WaveFileHeader.lAvgBytesPerSec  = WaveFileHeader.lSamplesPerSec * WaveFileHeader.lBlockAlign;
	// WaveFileHeader.lFileSize = NumData  + sizeof(WAVEFILEHEADER)-8;
	// WaveFileHeader.lDataSize = NumData;
  //
	// fwrite(&WaveFileHeader, sizeof(WAVEFILEHEADER), 1, fp);
  // NumData = 0;

  short BIT_RATE = 16;

  WAVE_HEADER header;
  memcpy(header.Riff.ChunkID, "RIFF", 4);
  header.Riff.ChunkSize = DURATION * SAMPLE_RATE * CHANNEL * BIT_RATE / 8 + 36;
  memcpy(header.Riff.Format, "WAVE", 4);

  memcpy(header.Fmt.ChunkID, "fmt ", 4);
  header.Fmt.ChunkSize = 0x10;
  header.Fmt.AudioFormat = WAVE_FORMAT_PCM;
  header.Fmt.NumChannels = CHANNEL;
  header.Fmt.SampleRate = SAMPLE_RATE;
  header.Fmt.AvgByteRate = SAMPLE_RATE * CHANNEL * BIT_RATE / 8;
  header.Fmt.BlockAlign = CHANNEL * BIT_RATE / 8;
  header.Fmt.BitPerSample = BIT_RATE;

  memcpy(header.Data.ChunkID, "data", 4);
  header.Data.ChunkSize = DURATION * SAMPLE_RATE * CHANNEL * BIT_RATE / 8;

  fwrite(&header, sizeof(header), 1, fp);


	return true;
}


void TTSNode::makeTTS(const string& sentence,const string& filepath) throw (Exception)
{
	try{
			void* pTTS=PTTS_CreateThread((void*)this, PCMCallBack, ttsInfo.language, ttsInfo.speaker);
			if(pTTS)
			{
        PTTS_ChangeSpeaker(pTTS, ttsInfo.speaker);
				PTTS_SetSpeed(pTTS, ttsInfo.speed);
        PTTS_SetVolume(pTTS, ttsInfo.volume);
				PTTS_SetPitch(pTTS, ttsInfo.pitch);

				if(ttsInfo.language == 1049){
					PTTS_SetHighLight( NULL, 1 );
					PTTS_SetLipSync( NULL, 1 );
				}else{
					PTTS_SetHighLight( pTTS, 1 );
					PTTS_SetLipSync( pTTS, 1 );
				}

				PTTS_SetCharSet( pTTS, 1 );
			}else{
				fclose(fp);
				Exception e;
				throw e;
			}

//			if(m_encoding == (int)Encoding::EUC_KR){
//				_bstr_t wSentence(sentence.c_str());
//				int nLen = WideCharToMultiByte( (unsigned int)CP_ACP, (unsigned long)0L, (const wchar_t*)wSentence, -1, NULL, 0, NULL, NULL );
//				cha                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          r* szText = new char[nLen];
//				char* szTextUTF8 = new char[nLen*2];
//				WideCharToMultiByte( (unsigned int)CP_ACP, 0, (const wchar_t*)wSentence, -1, szText, nLen, NULL, NULL );
//				ConvertANSIToUTF8(szText, szTextUTF8);
//				PTTS_TextToSpeech(pTTS, szTextUTF8, 1);
//				delete[] szText;
//				delete[] szTextUTF8;
//
//			}else if(m_encoding == (int)Encoding::UTF_8){
//
//			}
			PTTS_TextToSpeech(pTTS, (char*)sentence.c_str(), 1);
			if(pTTS){
				PTTS_DeleteThread(pTTS);
			}
			makeWavFile(filepath, m_TTSResTotal);

	} catch(Exception& e) {
		e.setMessage("PTTS Thread Loading Error - License Problem ");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}


void TTSNode::setProertiesCB(const silbot3_msgs::TTSSetProperties::ConstPtr& properties)
{

  // # Speaker id constants
  // uint8 SPEAKER_ID_KOREAN_FEMALE = 0
  // uint8 SPEAKER_ID_KOREAN_MALE = 1
  // uint8 SPEAKER_ID_KOREAN_KID = 2
  // uint8 SPEAKER_ID_ENGLISH_FEMALE = 3
  // uint8 SPEAKER_ID_ENGLISH_MALE = 4
  // uint8 SPEAKER_ID_DANISH_FEMALE = 5
  // uint8 SPEAKER_ID_FINNISH_FEMALE = 6
  // uint8 SPEAKER_ID_CHINESE_FEMALE = 7
  // uint8 SPEAKER_ID_CHINESE_MALE = 8
  //
  // uint8 speaker_id
  // uint8 speed
  // uint8 volume
  // uint8 pitch

  ROS_INFO("setProertiesCB {speaker_id : %d, speed : %d, volume : %d, pitch : %d}", properties->speaker_id, properties->speed, properties->volume, properties->pitch);

  // int language;
  // int speaker;
  // int volume;
  // int speed;
  // int pitch;

  switch(properties->speaker_id) {
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_KOREAN_FEMALE:
      ttsInfo.speaker = 0;
      #ifdef ACA_TTS
  		  ttsInfo.language = PTTS_KO_KR;
      #else
        ttsInfo.language = 0;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_KOREAN_MALE:
      ttsInfo.speaker = 3;
      #ifdef ACA_TTS
        ttsInfo.language = PTTS_KO_KR;
      #else
        ttsInfo.language = 0;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_KOREAN_KID:
      ttsInfo.speaker = 2;
      #ifdef ACA_TTS
  		  ttsInfo.language = PTTS_KO_KR;
      #else
        ttsInfo.language = 0;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_ENGLISH_FEMALE:
      ttsInfo.speaker = 4;
      #ifdef ACA_TTS
  		  ttsInfo.language = PTTS_EN_US;
      #else
        ttsInfo.language = 1;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_ENGLISH_MALE:
      ttsInfo.speaker = 1;
      #ifdef ACA_TTS
        ttsInfo.language = PTTS_EN_US;
      #else
        ttsInfo.language = 1;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_DANISH_FEMALE:
      ttsInfo.speaker = 1;
      ttsInfo.language = 102;
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_FINNISH_FEMALE:
      ttsInfo.speaker = 0;
      ttsInfo.language = 110;
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_CHINESE_FEMALE:
      ttsInfo.speaker = 0;
      #ifdef ACA_TTS
        ttsInfo.language = PTTS_ZH_CN;
      #else
        ttsInfo.language = 2;
      #endif
      break;
    case silbot3_msgs::TTSSetProperties::SPEAKER_ID_CHINESE_MALE:
      ttsInfo.speaker = 1;
      #ifdef ACA_TTS
        ttsInfo.language = PTTS_ZH_CN;
      #else
        ttsInfo.language = 2;
      #endif
      break;
    default:

    break;
  }

  ttsInfo.speed = properties->speed;
  ttsInfo.pitch = properties->pitch;
  ttsInfo.volume = properties->volume;


}

int TTSNode::processPCMCallback(SYNRES* pSynRes, TTSNode* pTTS)  {
	if(pSynRes != NULL && pSynRes->NumBytes>0) {
		if(pSynRes->pData != NULL){
			fwrite(pSynRes->pData, 1, pSynRes->NumBytes, pTTS->fp);
			m_TTSResTotal += pSynRes->NumBytes;
		}

		if( pSynRes->pVoiceStatus && pSynRes->pVoiceStatus->uIPhonemeNum>0 ){
			VISEMES visemes;
			for(unsigned int i = 0; i < pSynRes->pVoiceStatus->uIPhonemeNum; i++ ){
				visemes.VisemeID = pSynRes->pVoiceStatus->VisemeID[ i ].VisemeID;
				visemes.nDur =	 pSynRes->pVoiceStatus->VisemeID[ i ].nDur;
				visemes.chStress = pSynRes->pVoiceStatus->VisemeID[i].chStress;
				visemes.nStPos = pSynRes->pVoiceStatus->VisemeID[i].nStPos;
				if(pSynRes->pVoiceStatus->VisemeID[ i ].nDur != 0)
					m_svVisemes.push_back(visemes);
			}
		}

	}else if(pSynRes && pSynRes->NumBytes<=0 && pSynRes->pData != NULL && pSynRes->Status == 1 && pSynRes->pVoiceStatus && pSynRes->pVoiceStatus->uIPhonemeNum > 0){
		VISEMES visemes;
		for(unsigned int i = 0; i < pSynRes->pVoiceStatus->uIPhonemeNum; i++ ){
			visemes.VisemeID = pSynRes->pVoiceStatus->VisemeID[ i ].VisemeID;
			visemes.nDur =	 pSynRes->pVoiceStatus->VisemeID[ i ].nDur;
			visemes.chStress = pSynRes->pVoiceStatus->VisemeID[i].chStress;
			visemes.nStPos = pSynRes->pVoiceStatus->VisemeID[i].nStPos;
			if(pSynRes->pVoiceStatus->VisemeID[ i ].nDur != 0)
				m_svVisemes.push_back(visemes);
		}
	}else {
		//ROS_WARN("TTS.cpp::PCM callback not operated.");
	}

	return 0;
}

int TTSNode::PCMCallBack( void *pInParam,SYNRES *pSynRes )
{
	TTSNode* ttsHCI = (TTSNode*)(((pInParam)));
    return ttsHCI->processPCMCallback(pSynRes, ttsHCI);
}

int main(int argc, char **argv) {
  try {
    ros::init(argc, argv, "silbot3_tts");
    tts::TTSNode node;
    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::waitForShutdown();
  } catch(Exception& e) {
    e.printStackTrace();
  }
}
