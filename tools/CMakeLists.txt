find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv4)
pkg_check_modules(CONFIGPP REQUIRED libconfig++)

include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_BINARY_DIR}/)
link_libraries(NeguPi)

#add_executable(exposurecomp exposurecomp.cc)
#target_include_directories(exposurecomp PUBLIC ${OPENCV_INCLUDE_DIRS})
