cmake_minimum_required(VERSION 2.8.3)
project(silbot3_common_tools)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sensor_msgs roslib silbot3_msgs cmake_modules)
find_package(TinyXML REQUIRED)

catkin_package(
  INCLUDE_DIRS src
  LIBRARIES silbot3_common_tools
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs  silbot3_msgs
  DEPENDS TinyXML
)

MESSAGE("${TinyXML_INCLUDE_DIRS}")

add_definitions(-D__GNUC_)

include_directories(
${catkin_INCLUDE_DIRS}
${catkin_INCLUDE_DIRS}
${PROJECT_SOURCE_DIR}/src
${TinyXML_INCLUDE_DIRS}
)

link_directories(${catkin_LIBRARY_DIRS})

add_library(silbot3_common_tools
	src/robocare/util/getch.cpp
	src/robocare/exception/ParseException.cpp
	src/robocare/exception/TimeoutException.cpp
	src/robocare/exception/ArrayOutOfBoundException.cpp
	src/robocare/exception/ConnectException.cpp
	src/robocare/exception/NullPointerException.cpp
	src/robocare/exception/IOException.cpp
	src/robocare/exception/FileNotFoundException.cpp
	src/robocare/exception/NoSuchElementException.cpp
	src/robocare/exception/NumberFormatException.cpp
	src/robocare/exception/Exception.cpp
	src/robocare/device/data/BatteryData.cpp
	src/robocare/device/data/PanTiltPosition.cpp
	src/robocare/device/data/GyroData.cpp
	src/robocare/device/data/UltrasonicData.cpp
	src/robocare/device/data/WheelEncoderData.cpp
	src/robocare/device/data/ArmData.cpp
	src/robocare/device/data/TouchButtonData.cpp
	src/robocare/device/client/CBatteryClient.cpp
	src/robocare/device/client/CTouchButtonClient.cpp
	src/robocare/device/client/CWheelClient.cpp
	src/robocare/device/client/DeviceClient.cpp
	src/robocare/device/client/CGyroClient.cpp
	src/robocare/device/client/CArmClient.cpp
	src/robocare/device/client/CPanTiltClient.cpp
	src/robocare/device/client/CLightEmittingDiodeClient.cpp
	src/robocare/device/client/CWheelEncoderClient.cpp
	src/robocare/device/client/CUltraClient.cpp
)
target_link_libraries(silbot3_common_tools ${catkin_LIBRARIES} ${TinyXML_LIBRARIES})

add_dependencies(silbot3_common_tools silbot3_msgs_generate_messages_cpp)

install(TARGETS silbot3_common_tools
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

#install(DIRECTORY include/robocare/  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}  FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/robocare/  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}  FILES_MATCHING PATTERN "*.h")
#install(DIRECTORY config  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION} )

