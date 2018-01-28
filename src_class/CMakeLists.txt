cmake_minimum_required (VERSION 2.6)
project (PiNet)

# The version number.
set (PiNet_VERSION_MAJOR 1)
set (PiNet_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/pinet.h.in"
  "${PROJECT_BINARY_DIR}/pinet.h"
  )

# add the binary tree to the search path for include files
# so that we will find pinet.h
include_directories ("${PROJECT_BINARY_DIR}")
set(SOURCES pinet.cpp main.cpp)
#add_executable(test ${SOURCES})
#set ( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ~/cpp-netlib-build-raspberrypi )
find_package ( cppnetlib 0.11.0 REQUIRED )
include_directories ( ${CPPNETLIB_INCLUDE_DIRS} )

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -pthread")
endif()

# add the executable
add_executable (PiNet pinet.cpp main.cpp)
target_link_libraries (PiNet ${CPPNETLIB_LIBRARIES} )

# add the install targets
install (TARGETS PiNet DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/pinet.h" 
	 DESTINATION include)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
add_library( pinet.cpp main.cpp ${CMAKE_CURRENT_BINARY_DIR}/pinet.h  )


