cmake_minimum_required(VERSION 2.8)

project(${PROJECT_NAME} C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

#add_executable(attack-graph-generator
#        ag_network.c ag_asset.c db_util.c)
#target_link_libraries(attack-graph-generator)

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIR})
add_executable(db_test
        db_test.c ag_network.c ag_asset.c ag_facts.c db_util.c)
target_link_libraries(db_test ${PostgreSQL_LIBRARIES})
