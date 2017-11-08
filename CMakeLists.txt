cmake_minimum_required(VERSION 2.8)
project(ag_gen)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/extra")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -g -fopenmp")

find_package(BISON)
find_package(FLEX)
find_package(TBB)

find_library(POSTGRESQL_LIB pq /usr/local/lib)

BISON_TARGET(nm_parser src/compiler/nm_parser.yy
    ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/nm_parser.tab.h)
FLEX_TARGET(nm_scanner src/compiler/nm_scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/nm_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(nm_scanner nm_parser)

file(GLOB ag_gen_src
        "src/*.cpp"
        "src/*.h")

# Main application
add_executable(ag_gen ${ag_gen_src})
target_include_directories(ag_gen PRIVATE ${TBB_INCLUDE_DIRS})
target_compile_definitions(ag_gen PRIVATE ${TBB_DEFINITIONS})
target_link_libraries(ag_gen ${POSTGRESQL_LIB} ${TBB_LIBRARIES})

# include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/compiler)
# add_executable(nm_test
#     ${FLEX_nm_scanner_OUTPUTS}
#     ${BISON_nm_parser_OUTPUTS}
#     src/compiler/str_util.c
#     src/compiler/util_hash.c
#     src/compiler/build_sql.c)

# # Coding examples
# add_executable(graph_example "examples/graph_example.cpp")
# add_executable(db_api_example "examples/db_api_example.cpp")

# # Tools and Work-in-Progress Ideas
# add_executable(sha256_test "tools/sha256_test.cpp")
# add_executable(graphing_db "tools/graphing_db.cpp")

# include_directories(
#     "/usr/local/include"
#     "/usr/include/postgresql"
# )

# if(APPLE)
#     include_directories(
#         "/usr/local/opt/openssl/include"
#         "/usr/local/Cellar/tbb/2018_U1/include"
#     )
# endif()


# find_library(OPENSSL_LIB ssl /usr/local/opt/openssl/lib)
# find_library(OPENSSL_CRYPTO_LIB crypto /usr/local/opt/openssl/lib)
# target_link_libraries(sha256_test ${OPENSSL_LIB} ${OPENSSL_CRYPTO_LIB})

# target_link_libraries(graph_example ${POSTGRESQL_LIB})

# target_link_libraries(db_api_example ${POSTGRESQL_LIB})

# target_link_libraries(graphing_db ${POSTGRESQL_LIB})
