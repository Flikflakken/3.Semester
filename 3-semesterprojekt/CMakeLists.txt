cmake_minimum_required(VERSION 3.5)

project(3-semesterprojekt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
find_package(Pylon REQUIRED )
include_directories(${Pylon_INCLUDE_DIRS})

add_executable(3-semesterprojekt main.cpp camera.cpp camera.h camera_cali.h camera_cali.cpp picture_handling.h picture_handling.cpp)

target_link_libraries( 3-semesterprojekt ${Pylon_LIBRARIES})
target_link_libraries( 3-semesterprojekt ${OpenCV_LIBS} )
