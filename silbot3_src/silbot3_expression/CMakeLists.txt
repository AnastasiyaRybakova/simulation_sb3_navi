cmake_minimum_required(VERSION 2.8.3)
project(silbot3_expression)

find_package(catkin REQUIRED COMPONENTS cmake_modules roscpp sensor_msgs std_msgs roslib silbot3_common_tools)
find_package(TinyXML REQUIRED)

catkin_package(
  INCLUDE_DIRS src
  LIBRARIES silbot3_expression
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs tf silbot3_common_tools
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${TinyXML_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

link_directories(${catkin_LIBRARY_DIRS} )

add_executable(silbot3_expression
		src/expression/protocol/ExpressionParameter.cpp
		src/expression/ExpressionNode.cpp
		src/expression/Expression.cpp
		src/expression/exception/InvalidProtocolException.cpp
		src/expression/exception/CannotChangeStatusException.cpp
		src/expression/exception/CannotPlayException.cpp
		src/expression/exception/Exception.cpp
		src/expression/exception/InvalidProtocolException.cpp
		src/expression/exception/IOException.cpp
		src/expression/exception/NoSuchElementException.cpp
		src/expression/exception/ParseException.cpp
    src/expression/exception/ArrayOutofBoundsException.cpp
		src/expression/modality/ModalityUseSet.cpp
		src/expression/modality/Modality.cpp
		src/expression/utility/ofUtil.cpp
		src/expression/utility/Parameter.cpp
		src/expression/utility/ofxXmlSettings.cpp
		src/expression/utility/ofLog.cpp
		src/expression/utility/SentenceTokenizer.cpp
		src/expression/player/ExpressionWithSongPlayer.cpp
		src/expression/player/ExpressionWithoutMotionPlayer.cpp
		src/expression/player/ExpressionWithMotionPlayer.cpp
		src/expression/player/ExpressionPlayer.cpp
		src/expression/db/ExpressionDB.cpp
		src/expression/db/ExpressionSequence.cpp
		src/expression/db/ExpressionDBLoader.cpp
    src/expression/db/VisemeInfoParser.cpp
    src/expression/db/RecordedVisemeInfo.cpp
		src/expression/status/Status.cpp
		src/expression/status/Context.cpp
		src/expression/connector/StatusPublisher.cpp
		src/expression/connector/DeviceConnector.cpp
		src/expression/connector/AvatarConnector.cpp
		src/expression/connector/SoundConnector.cpp
		src/expression/lipsync/Lipsync.cpp
		src/expression/lipsync/RecordedSentenceInfo.cpp
		src/expression/lipsync/VisemeInfo.cpp
)

target_link_libraries(silbot3_expression ${catkin_LIBRARIES})
install(TARGETS silbot3_expression  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

add_dependencies(silbot3_expression silbot3_msgs_generate_messages_cpp)
