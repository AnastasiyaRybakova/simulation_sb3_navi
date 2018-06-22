cmake_minimum_required(VERSION 2.8.3)
project(silbot3_tts)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  silbot3_msgs
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES silbot3_tts
  DEPENDS roscpp silbot3_msgs
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

link_directories(
  lib
  ${catkin_LIBRARY_DIRS}
)

## Declare a C++ executable
add_executable(silbot3_tts
  src/Exception.cpp
  src/tts_node.cpp
)


add_dependencies(silbot3_tts silbot3_msgs_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(silbot3_tts
  ${catkin_LIBRARIES}
  m pthread dl
	PowerTTS_M
	MVoice
	EmVoice
	CmVoice
	Parsing
	common
	KMorph
	pthread
	hasp_linux_x86_64_92843
)

#############
## Install ##
#############

install(TARGETS silbot3_tts
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
