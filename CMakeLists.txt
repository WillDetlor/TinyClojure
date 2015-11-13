cmake_minimum_required(VERSION 3.3)
project(TinyClojure)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/TinyClojure.cpp
    src/TinyClojure.h
    src/tool.cpp)

add_executable(TinyClojure ${SOURCE_FILES})