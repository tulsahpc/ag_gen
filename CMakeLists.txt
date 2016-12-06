cmake_minimum_required(VERSION 3.6)
project(ag_gen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic -lhiredis -lm -lpq --std=c++14")

set(SOURCE_FILES
    src/ag_gen.cpp
    src/asset.cpp
    src/asset.hpp
    src/exploit.cpp
    src/exploit.hpp
    src/fact.hpp
    src/network.cpp
    src/network.hpp
    src/network_state.cpp
    src/quality.cpp
        src/redisasset.cpp
    src/redisasset.h
        src/redisexploit.cpp
    src/redisexploit.h
        src/util_common.cpp
    src/util_common.h
        src/util_db.cpp
    src/util_db.h
        src/util_odometer.cpp
    src/util_odometer.h
    src/util_redis.cpp
        src/util_redis.h
    src/hashmap.cpp
        src/hashmap.h)

add_executable(ag_gen ${SOURCE_FILES})