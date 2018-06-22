cmake_minimum_required(VERSION 2.8.3)
project(silbot3_core)

find_package( catkin REQUIRED COMPONENTS
	cmake_modules
	roscpp
	sensor_msgs
	std_msgs
	cmake_modules
	tf
	silbot3_common_tools
	nav_msgs
	kdl_parser
	tf_conversions
)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

catkin_package(
  LIBRARIES silbot3_core
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs silbot3_common_tools tf nav_msgs
  CFG_EXTRAS
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/../silbot3_common_tools/src
)

link_directories(
	${catkin_LIBRARY_DIRS}
)

add_executable(silbot3_core
	src/reactive/erobot/session/EBatteryCore.cpp
	src/reactive/erobot/session/EUltraCore.cpp
	src/reactive/erobot/session/EArmCore.cpp
	src/reactive/erobot/session/ETouchCore.cpp
	src/reactive/erobot/session/EWheelCore.cpp
	src/reactive/erobot/session/EGyroCore.cpp
	src/reactive/erobot/session/EWheelEncoderCore.cpp
	src/reactive/erobot/session/EPantiltCore.cpp
	src/reactive/erobot/session/ELEDCore.cpp
	src/reactive/erobot/session/ERobotSerialConnector.cpp
	src/reactive/erobot/session/EBodyCoreXtion.cpp
	src/reactive/erobot/session/EBodyCoreKinect.cpp
	src/reactive/erobot/ERobotCore.cpp
	src/reactive/erobot/utility/ERobotFunctionFactory.cpp
	src/device/odif/wheel/WheelERobot.cpp
	src/device/odif/pantilt/PanTiltERobot.cpp
	src/device/odif/arm/ArmERobot.cpp
	src/device/odif/led/LEDERobot.cpp
	src/device/server/handler/BatteryHandler.cpp
	src/device/server/handler/WheelHandler.cpp
	src/device/server/handler/GyroHandler.cpp
	src/device/server/handler/WheelEncoderHandler.cpp
	src/device/server/handler/PantiltHandler.cpp
	src/device/server/handler/TouchHandler.cpp
	src/device/server/handler/UltrasonicHandler.cpp
	src/device/server/handler/ArmHandler.cpp
	src/device/server/handler/LEDHandler.cpp
	src/device/server/robot_setup/OdomSetup.cpp
	src/device/server/robot_setup/JointSetup.cpp
	src/device/idif/battery/BatteryERobot.cpp
	src/device/idif/touch/TouchButtonERobot.cpp
	src/device/idif/ultrasonic/UltrasonicERobot.cpp
	src/device/idif/gyro/GyroERobot.cpp
	src/device/idif/wheelencoder/WheelEncoderERobot.cpp
	src/device/exception/Exception.cpp
	src/device/exception/IOException.cpp
	src/device/server/DeviceServer.cpp
)

add_dependencies(silbot3_core silbot3_msgs_generate_messages_cpp)

target_link_libraries(silbot3_core ${catkin_LIBRARIES} ${silbot3_common_tools_LIBRARIES} )

install(TARGETS silbot3_core
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY param DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
