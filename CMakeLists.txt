cmake_minimum_required(VERSION 2.8)
project(ag_gen)

#set(CMAKE_C_COMPILER "/usr/local/bin/gcc-6")
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-6")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -Wall -pedantic")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -Wall -pedantic -O1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")

file(COPY "${PROJECT_SOURCE_DIR}/config_default.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/config_default.txt" "${CMAKE_CURRENT_BINARY_DIR}/config.txt")

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

find_package(BISON)
find_package(FLEX)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL)
find_package(Boost)

include_directories("src/")

file(GLOB ag_gen_src "src/ag_gen/*.cpp")
file(GLOB utils_src "src/util/*.c" "src/util/*.cpp")

# Main application
add_executable(ag_gen "src/main.cpp" ${ag_gen_src} ${utils_src})
target_link_libraries(ag_gen ${PostgreSQL_LIBRARIES})

# Parser
BISON_TARGET(nm_parser src/compiler/nm_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.tab.h)
FLEX_TARGET(nm_scanner src/compiler/nm_scanner.l ${CMAKE_CURRENT_BINARY_DIR}/nm_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(nm_scanner nm_parser)

add_executable(nm_test ${FLEX_nm_scanner_OUTPUTS} ${BISON_nm_parser_OUTPUTS} ${utils_src})
target_include_directories(nm_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/compiler/nm-parser)

# Coding examples
add_executable(graph_example "src/examples/graph_example.cpp")

add_executable(db_api_example "src/examples/db_api_example.cpp")
target_link_libraries(db_api_example ${PostgreSQL_LIBRARIES})

# Tools and Work-in-Progress Ideas
add_executable(sha256_test "src/tools/sha256_test.cpp")
target_include_directories(sha256_test PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(sha256_test ${OPENSSL_LIBRARIES})

add_executable(graphing_db "src/tools/graphing_db.cpp")
target_link_libraries(graphing_db ${BOOST_LIBRARIES} ${PostgreSQL_LIBRARIES})
