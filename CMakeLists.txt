cmake_minimum_required(VERSION 3.29)
project(HIVE)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HIVE
    .gitignore
    insect.cpp
    insect.h
    map.cpp
    map.h
    utils.cpp
    utils.h)
