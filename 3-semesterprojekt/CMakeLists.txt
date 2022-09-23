cmake_minimum_required(VERSION 3.5)

project(3-semesterprojekt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( PROJECT_SOURCE_DIR
    main.cpp
    camara.cpp)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(3-semesterprojekt main.cpp)

target_link_libraries( 3-semesterprojekt ${OpenCV_LIBS} )
