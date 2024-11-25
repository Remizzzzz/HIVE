cmake_minimum_required(VERSION 3.26)
project(HIVE)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HIVE
        insect.cpp
        insect.h
        main.cpp
        map.cpp
        map.h
        utils.cpp
        utils.h
        hive.h
        deck.h)
