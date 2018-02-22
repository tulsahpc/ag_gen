cmake_minimum_required(VERSION 2.8)
project(ag_gen)

# Common compiler options among built types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Specific compiler options for Debug or Release builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -ggdb -Wall -pedantic")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -Wall -pedantic -O1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O1")

set(PostgreSQL_ADDITIONAL_VERSIONS "10.1" "10")

# Copy the config file to the build directory
file(COPY "${CMAKE_SOURCE_DIR}/config_default.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/config_default.txt" "${CMAKE_CURRENT_BINARY_DIR}/config.txt")

# Apple has a different openssl directory when using brew
#if(APPLE)
#    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
#endif()

# Find packages
find_package(PostgreSQL REQUIRED)
find_package(OpenMP)

# Only required for supplementary tools
find_package(BISON)
find_package(FLEX)
find_package(OpenSSL)
find_package(Boost)
find_package(Doxygen)

if(PostgreSQL_FOUND)
    include_directories("${PostgreSQL_INCLUDE_DIR}")
endif()

# Enable thread-level parallelization if OpenMP is found.
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif(DOXYGEN_FOUND)

include_directories("${CMAKE_SOURCE_DIR}/src/")

file(GLOB ag_gen_src "${CMAKE_SOURCE_DIR}/src/ag_gen/*.cpp")
file(GLOB utils_src "${CMAKE_SOURCE_DIR}/src/util/*.c" "${CMAKE_SOURCE_DIR}/src/util/*.cpp")

# Main application
add_executable(ag_gen "${CMAKE_SOURCE_DIR}/src/main.cpp" ${ag_gen_src} ${utils_src})
target_link_libraries(ag_gen ${PostgreSQL_LIBRARIES} ${OpenSSL_LIBRARIES})

# Parser
BISON_TARGET(nm_parser "${CMAKE_SOURCE_DIR}/src/parser/nm-parser/nm_parser.yy" "${CMAKE_CURRENT_BINARY_DIR}/nm_parser.c" DEFINES_FILE "${CMAKE_CURRENT_BINARY_DIR}/nm_parser.tab.h")
FLEX_TARGET(nm_scanner "${CMAKE_SOURCE_DIR}/src/parser/nm-parser/nm_scanner.l" "${CMAKE_CURRENT_BINARY_DIR}/nm_scanner.c")
ADD_FLEX_BISON_DEPENDENCY(nm_scanner nm_parser)

# Network Model parser
add_executable(nm_test ${FLEX_nm_scanner_OUTPUTS} ${BISON_nm_parser_OUTPUTS} ${utils_src})
target_include_directories(nm_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_SOURCE_DIR}/src/compiler/nm-parser")

# Coding examples
add_executable(graph_example "${CMAKE_SOURCE_DIR}/src/examples/graph_example.cpp")

add_executable(db_api_example "${CMAKE_SOURCE_DIR}/src/examples/db_api_example.cpp")
target_link_libraries(db_api_example ${PostgreSQL_LIBRARIES})

# Tools and Work-in-Progress Ideas
add_executable(sha256_test "${CMAKE_SOURCE_DIR}/src/tools/sha256_test.cpp")
target_link_libraries(sha256_test ${OPENSSL_LIBRARIES})

add_executable(graphing_db "${CMAKE_SOURCE_DIR}/src/tools/graphing_db.cpp")
target_link_libraries(graphing_db ${BOOST_LIBRARIES} ${PostgreSQL_LIBRARIES})

add_executable(netgen "${CMAKE_SOURCE_DIR}/src/tools/netgen.cpp")
