cmake_minimum_required (VERSION 3.9)
project(NeguPi VERSION 2.1 DESCRIPTION "NeguPi")

set (CMAKE_CXX_STANDARD 17)

EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
message(STATUS "Architecture: ${ARCHITECTURE}")

if(NOT ${ARCHITECTURE} STREQUAL "armv7l")
	add_definitions(-DNODEVICE)
    set(NODEVICE 1)
endif()

add_library(NeguPi SHARED
	src/NeguPiLogger.cc
	src/NeguPiDaemon.cc
	src/VMCP23S17.cc
	src/MCP23S17Fake.cc
	src/MCP23S17.cc
	src/PiFace.cc
	src/PiFaceStateMachine.cc
)
         
install(FILES
	src/NeguPiLogger.h
	src/NeguPiDaemon.h
	src/VMCP23S17.h
	src/MCP23S17Fake.h
	src/MCP23S17.h
	src/PiFace.h
	src/PiFaceStateMachine.h
	DESTINATION include)

target_include_directories(NeguPi PRIVATE src)

set_target_properties(NeguPi PROPERTIES VERSION ${PROJECT_VERSION})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/NeguPi.h.in"
  "${PROJECT_BINARY_DIR}/NeguPi.h"
)

# add the binary tree to the search path for include files
# so that we will find NeguPi.h
include_directories ("${PROJECT_BINARY_DIR}")

# add the install targets
install (TARGETS NeguPi DESTINATION lib)
install (FILES "${PROJECT_BINARY_DIR}/NeguPi.h"        
         DESTINATION include)
