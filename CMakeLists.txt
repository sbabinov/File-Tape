cmake_minimum_required(VERSION 3.14)
project(FileTape)

set(CMAKE_CXX_STANDARD 17)

option(FILE_TAPE_BUILD_TESTS "Build tests" ON)

add_executable(main src/main.cpp src/tapes.cpp src/converter.cpp src/delimiters.cpp)

if(FILE_TAPE_BUILD_TESTS)
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()
