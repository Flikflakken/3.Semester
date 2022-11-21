cmake_minimum_required(VERSION 3.5)

project(3-semesterprojekt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(Pylon REQUIRED )
include_directories(${Pylon_INCLUDE_DIRS})

find_package(ur_rtde REQUIRED PATHS "/home/mikkel/ur_rtde/ur_rtde/build/ur_rtde")
include_directories(${ur_rtde_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(3-semesterprojekt
        main.cpp
        camera.cpp camera.h
        camera_cali.h camera_cali.cpp
        picture_handling.h picture_handling.cpp
        gripper.h gripper.cpp
        tcp.cpp tcp.h
        robotController.cpp robotController.h)

target_link_libraries( 3-semesterprojekt PRIVATE
    ${Pylon_LIBRARIES}
    ${OpenCV_LIBS}
    ur_rtde::rtde
    Qt5::Network
    Qt5::Core)

