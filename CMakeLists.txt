cmake_minimum_required(VERSION 3.13)
project(KPSCounter)

set(CMAKE_CXX_STANDARD 14)

add_executable(KPSCounter main.cpp KeyPresses.cpp KeyPresses.h)

FIND_PACKAGE(FLTK REQUIRED)
TARGET_LINK_LIBRARIES(KPSCounter ${FLTK_LIBRARIES})

if (UNIX)
    TARGET_LINK_LIBRARIES(KPSCounter pthread)
endif (UNIX)