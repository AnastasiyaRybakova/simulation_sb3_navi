cmake_minimum_required(VERSION 2.8.3)
project(silbot3_navigation)

find_package(catkin REQUIRED COMPONENTS 
	roscpp sensor_msgs std_msgs move_base_msgs nav_msgs actionlib silbot3_common_tools cmake_modules
)
find_package(TinyXML REQUIRED)

catkin_package(
   INCLUDE_DIRS src
   LIBRARIES silbot3_navigation
   CATKIN_DEPENDS roscpp sensor_msgs std_msgs silbot3_common_tools nav_msgs actionlib move_base_msgs
)

include_directories(include 
	${catkin_INCLUDE_DIRS}
	${TinyXML_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src
	/home/robocare/robocare_ws/src/silbot3_common_tools/src
)

## Declare a C++ library
# add_library(silbot3_navigation
#   src/${PROJECT_NAME}/silbot3_navigation.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
##add_dependencies(silbot3_navigation ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(silbot3_navigation_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against



## Declare a C++ executable
add_executable(silbot3_navigation src/silbot3_navigation/silbot3_navigation.cpp)


## Specify libraries to link a library or executable target against
target_link_libraries(silbot3_navigation
   ${catkin_LIBRARIES}
   ${silbot3_common_tools_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS silbot3_navigation silbot3_navigation_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_silbot3_navigation.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
