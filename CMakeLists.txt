cmake_minimum_required(VERSION 3.26)
project(HIVE)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HIVE
        main.cpp
        src/utils/hiveException.h
        src/utils/utils.cpp
        src/utils/utils.h
        src/features/insect.cpp
        src/features/insect.h
        src/features/map.cpp
        src/features/map.h
        src/features/deck.h
        src/features/inputs.h
        src/inputsManager.h
        src/solver.h
        src/renderer.h
        src/hive.h
        src/features/inputsManager.cpp)
