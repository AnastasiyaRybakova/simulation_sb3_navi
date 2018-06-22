# Silbot3 TextToSpeech node
* Generate speech sound file from text
* Generate viseme data from text

## Usage

### Set properties
* Send message to **/silbot3_tts/set_properties** with [**silbot3_msgs/TTSSetProperties**](silbot3_msgs/msg/TTSSetProperties.msg) type
* Example
  * ```rostopic pub /silbot3_tts/set_properties silbot3_msgs/TTSSetProperties "speaker_id: 0 speed: 0 volume: 0 pitch: 0"```

### Generate speech sound file and viseme data
* Call make service to **/silbot3_tts/make** with [**silbot3_srvs/TTSMake**](silbot3_msgs/srv/TTSMake.srv) type
* Array of Viseme data will included in response message. Check [TTSViseme](silbot3_msgs/msg/TTSViseme.msg) definition
* Example
  * ```rosservice call /silbot3_tts/make "text: 'text-to-speech' filepath: 'path-to-save-file'"```

