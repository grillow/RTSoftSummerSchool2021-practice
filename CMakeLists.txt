cmake_minimum_required(VERSION 3.18.4)
project( RTSoftSummerSchool2021-practice )

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB SOURCE src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
