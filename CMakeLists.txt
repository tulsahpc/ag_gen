cmake_minimum_required(VERSION 3.6)
project(ag_gen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic --std=c++14")

file(GLOB ag_gen_src
        "src/*.cpp"
        "src/*.hpp")

add_executable(ag_gen ${ag_gen_src} src/quality.h src/topology.h src/assetgroup.cpp src/assetgroup.h src/parameterized_quality.h src/parameterized_topology.h)

include_directories("/usr/local/include")

find_library(HIREDIS_LIB hiredis /usr/local/lib)
find_library(POSTGRESQL_LIB pq /usr/local/lib)
target_link_libraries(ag_gen ${HIREDIS_LIB} ${POSTGRESQL_LIB})
