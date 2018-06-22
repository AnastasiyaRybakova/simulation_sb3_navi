cmake_minimum_required(VERSION 2.8.3)
project(silbot3_avatar)

find_package(catkin REQUIRED COMPONENTS roscpp roslib silbot3_common_tools cmake_modules)
find_package(TinyXML REQUIRED)

catkin_package(
  LIBRARIES silbot3_avatar
  CATKIN_DEPENDS roscpp roslib
  DEPENDS TinyXML
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${TinyXML_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

link_directories(
	${catkin_LIBRARY_DIRS}
)

add_executable(silbot3_avatar
	src/avatar/message/GameMessage.cpp
	src/avatar/message/MessageParser.cpp
	src/avatar/message/Parameter.cpp
	src/avatar/message/ParameterPart.cpp
	src/avatar/server/AvatarServer.cpp
	src/avatar/transport/MessageReceiver.cpp
	src/avatar/transport/MessageWriter.cpp
	src/avatar/transport/SessionMap.cpp
	src/avatar/transport/SessionTask.cpp
	src/avatar/connector/WServerSocket.cpp
	src/avatar/connector/WSocket.cpp
	src/avatar/util/UUID.cpp
	src/avatar/avatar.cpp
)

target_link_libraries(silbot3_avatar
  ${TinyXML_LIBRARIES}
  ${catkin_LIBRARIES}
)

install(DIRECTORY param DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(TARGETS silbot3_avatar
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

