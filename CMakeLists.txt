cmake_minimum_required(VERSION 2.8)
project(ag_gen)

# Add FindTBB (from github)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/extra")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g")

file(COPY "${PROJECT_SOURCE_DIR}/config_default.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/config_default.txt" "${CMAKE_CURRENT_BINARY_DIR}/config.txt")

file(GLOB ag_gen_src "src/*.cpp" "src/*.h")

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

find_package(BISON)
find_package(FLEX)
find_package(TBB REQUIRED)
find_package(POSTGRESQL REQUIRED)
find_package(OPENSSL)
find_package(BOOST)

# Main application
add_executable(ag_gen ${ag_gen_src})
target_include_directories(ag_gen PRIVATE ${TBB_INCLUDE_DIRS})
target_compile_definitions(ag_gen PRIVATE ${TBB_DEFINITIONS})
target_link_libraries(ag_gen ${PostgreSQL_LIBRARIES} ${TBB_LIBRARIES})

# Parser
BISON_TARGET(nm_parser src/compiler/nm_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.tab.h)
FLEX_TARGET(nm_scanner src/compiler/nm_scanner.l ${CMAKE_CURRENT_BINARY_DIR}/nm_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(nm_scanner nm_parser)

add_executable(nm_test ${FLEX_nm_scanner_OUTPUTS} ${BISON_nm_parser_OUTPUTS} src/compiler/str_util.c src/compiler/util_hash.c src/compiler/build_sql.c)
target_include_directories(nm_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/compiler)

# Coding examples
add_executable(graph_example "examples/graph_example.cpp")

add_executable(db_api_example "examples/db_api_example.cpp")
target_link_libraries(db_api_example ${PostgreSQL_LIBRARIES})

# Tools and Work-in-Progress Ideas
add_executable(sha256_test "tools/sha256_test.cpp")
target_include_directories(sha256_test PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(sha256_test ${OPENSSL_LIBRARIES})

add_executable(graphing_db "tools/graphing_db.cpp")
target_link_libraries(graphing_db ${BOOST_LIBRARIES} ${PostgreSQL_LIBRARIES})
