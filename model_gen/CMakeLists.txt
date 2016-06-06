cmake_minimum_required(VERSION 3.5)
project(netmodgen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -std=c++14")

set(SOURCE_FILES main.cpp netMod.cpp netMod.h atkPat.cpp atkPat.h)
add_executable(netmodgen ${SOURCE_FILES})
