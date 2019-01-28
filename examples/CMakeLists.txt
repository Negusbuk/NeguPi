find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv)
pkg_check_modules(CONFIGPP REQUIRED libconfig++)

find_package(Threads REQUIRED)

include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_BINARY_DIR}/)
link_libraries(NeguPi)

add_executable(blink blink.cc)

add_executable(statemachine statemachine.cc)

#add_executable(timelapse timelapse.cc)
#target_include_directories(timelapse PUBLIC ${CONFIGPP_INCLUDE_DIRS})
#target_include_directories(timelapse PUBLIC ${OPENCV_INCLUDE_DIRS})
#target_compile_options(timelapse PUBLIC ${CONFIGPP_CFLAGS_OTHER})
#target_compile_options(timelapse PUBLIC ${OPENCV_CFLAGS_OTHER})
#target_link_libraries(timelapse ${CONFIGPP_LIBRARIES})

#add_executable(videocapture videocapture.cc)
#target_include_directories(videocapture PUBLIC ${CONFIGPP_INCLUDE_DIRS})
#target_compile_options(videocapture PUBLIC ${CONFIGPP_CFLAGS_OTHER})
#target_link_libraries(videocapture ${CONFIGPP_LIBRARIES})

#add_executable(camerabox camerabox.cc)
#target_include_directories(camerabox PUBLIC ${CONFIGPP_INCLUDE_DIRS})
#target_include_directories(camerabox PUBLIC ${OPENCV_INCLUDE_DIRS})

#add_executable(recognition recognition.cc)
#target_include_directories(recognition PUBLIC ${CONFIGPP_INCLUDE_DIRS})
#target_include_directories(recognition PUBLIC ${OPENCV_INCLUDE_DIRS})
#target_link_libraries(recognition ${OPENCV_LIBRARIES})

add_executable(readGPS readGPS.cc)
target_link_libraries(readGPS ${CMAKE_THREAD_LIBS_INIT})

add_executable(mpu6050_1 mpu6050_1.cc)
