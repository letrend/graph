CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(graph C CXX)

# RelWithDebInfo, Release or Debug
SET(CMAKE_BUILD_TYPE Debug FORCE)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -std=c++11" )

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#==============================================================================
# Dependencies
#==============================================================================
# Eigen
find_package( PkgConfig )
pkg_check_modules( EIGEN3 REQUIRED eigen3 )
include_directories( ${EIGEN3_INCLUDE_DIRS} )

# OpenCV
FIND_PACKAGE(OpenCV COMPONENTS opencv_core opencv_highgui opencv_imgproc opencv_calib3d REQUIRED)

INCLUDE_DIRECTORIES(include)

#==============================================================================
# Application
#==============================================================================

ADD_EXECUTABLE(graph src/main.cpp)
TARGET_LINK_LIBRARIES(graph ${OpenCV_LIBS})

