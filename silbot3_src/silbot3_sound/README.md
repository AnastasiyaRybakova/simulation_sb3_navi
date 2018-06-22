# Silbot3 Sound player node
* Play sound file

### Requirement
* Install required packages if not exists  

```sh
sudo apt install python-gst-1.0 gstreamer1.0-libav
```

## Usage

### Play sound (Topic, asyncronous)
* Send play request message to **silbot3_sound/play** with **std_msgs/String** type
* Example
  * ```rostopic pub /silbot3_sound/play std_msgs/String "data:'/file/path/to/play.wav"```

### Play sound (Service, synchronous)
* Call play service to **silbot3_sound/play** with [**silbot3_srvs/SoundPlay**](silbot3_msgs/srv/SoundPlay.srv) type
* It will return play result
  * SOUND_RESULT_DONE
  * SOUND_RESULT_FILE_NOT_FOUND
  * SOUND_RESULT_FAILED
* Example
  * ```rosservice call /silbot3_sound/play "filepath: '/file/path/to/play.wav'"```

### Stop (Topic, asyncronous)
* Stop all playing sounds.
* Send Stop request message to **silbot3_sound/stop** with **std_msgs/Empty** type
* Example
  * ```rostopic pub /silbot3_sound/stop std_msgs/Empty```

