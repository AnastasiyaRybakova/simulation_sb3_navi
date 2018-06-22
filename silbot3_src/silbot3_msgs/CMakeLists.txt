cmake_minimum_required(VERSION 2.8.3)
project(silbot3_msgs)

find_package(catkin REQUIRED COMPONENTS roscpp message_generation)

add_message_files(
  DIRECTORY msg
  FILES
	Device_Arm_Msg.msg
	Device_Gyro_Msg.msg
	Device_Pantilt_Msg.msg
	Device_ErobotPantilt_Msg.msg
	Device_LED_Msg.msg
	Device_Touch_Msg.msg
	Device_Ultra_Msg.msg
	Device_WheelEncoder_Msg.msg
	Device_Wheel_Msg.msg
	Device_Battery_Msg.msg
	TTSSetProperties.msg
	TTSViseme.msg
	ExpressionSetModality.msg
	ExpressionStatus.msg
	DisplayMessage.msg
	PathFollow_GameResult.msg
	MotionFollow_GameResult.msg
	MotionFollow_Ment.msg
	MotionFollow_MotionResult.msg
)

add_service_files(
  DIRECTORY srv
  FILES
	Device_Arm_Srv.srv
	Device_Data_Srv.srv
	Device_Error_Srv.srv
	Device_Gyro_Srv.srv
	Device_Pantilt_Srv.srv
	Device_Ultra_Srv.srv
	Device_WheelEncoder_Srv.srv
	Device_Wheel_Srv.srv
	Device_LED_Srv.srv
	Device_Touch_Srv.srv
	ExpressionStart.srv
	AvatarRos_PlayLipSync_Srv.srv
	AvatarRos_Expression_Srv.srv
	AvatarRos_MoveFace_Srv.srv
	AvatarRos_FaceFactor_Srv.srv
	AvatarRos_SetWindow_Srv.srv
	AvatarRos_PlayMocapSong_Srv.srv
	AvatarRos_SetMode_Srv.srv
	AvatarRos_Dialog_Srv.srv
	CCM_Component_Status_Srv.srv
	CCM_Control_Srv.srv
	SoundPlay.srv
	TTSMake.srv
	PathFollow_RouteGeneration.srv
	Pose.srv
	MotionFollow_Init.srv
	MotionFollow_MotionGeneration.srv
	String.srv
	SpeechRecognitionStart.srv
)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
  CATKIN_DEPENDS roscpp rospy message_runtime std_msgs
)

include_directories(
  include ${catkin_INCLUDE_DIRS}
)
