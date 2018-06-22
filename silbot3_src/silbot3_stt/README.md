# Silbot3 SpeechToText node
* Generate text from speech

## Requirement
* Install required packages

```sh
sudo apt install python-pip pulseaudio portaudio19-dev
```
```sh
sudo -H pip install pyaudio SpeechRecognition
```

## Usage

### Speech to text
* Call start service to **/silbot3_stt/start** with [**silbot3_srvs/SpeechRecognitionStart**](silbot3_msgs/srv/SpeechRecognitionStart.srv) type
* Example
  * ```rosservice call /silbot3_stt/start "timeout:10"```

## Parameters
* operation_timeout (Default: 10) (sec)
  * Time for internal operations
* dynamic_threshold (Default: False)
  * Automatically adjust threshold
* pause_threshold (Default: 0.5) (sec)
  * Minimum length of silence
* energy_threshold (Default: 500)
  * Energy level threshold for sounds
* ambient_noise (Default: True)
  * Adjusts the energy threshold dynamically
* mic_name (Default: "default")
  * Check microphone device list and find name for correct device  

```python
# Python code for find microphone device lists.
import speech_recognition as sr
for index, name in enumerate(sr.Microphone.list_microphone_names()):
    print("Microphone with name \"{1}\" found for `device_index={0}`".format(index, name))
```
