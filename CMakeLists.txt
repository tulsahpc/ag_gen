cmake_minimum_required(VERSION 3.5)
project(ag_gen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall -DDEBUG_BUILD")

file(GLOB ag_gen_src
        "src/*.cpp"
        "src/*.h")

# Main application
add_executable(ag_gen ${ag_gen_src})

# Coding examples
#add_executable(graphing_test "examples/graph_example.cpp")
#add_executable(db_api_example "examples/db_api_example.cpp")

# Tools and Work-in-Progress Ideas
#add_executable(sha256_test "tools/sha256_test.cpp")
#add_executable(graphing_db "tools/graphing_db.cpp")

include_directories(
    "/usr/local/include"
    "/usr/include/postgresql"
)

if(APPLE)
    include_directories("/usr/local/opt/openssl/include")
endif()

find_library(POSTGRESQL_LIB pq /usr/local/lib)
target_link_libraries(ag_gen ${POSTGRESQL_LIB})

#find_library(OPENSSL_LIB ssl /usr/local/opt/openssl/lib)
#find_library(OPENSSL_CRYPTO_LIB crypto /usr/local/opt/openssl/lib)
#target_link_libraries(sha256_test ${OPENSSL_LIB} ${OPENSSL_CRYPTO_LIB})

#target_link_libraries(graphing_test ${POSTGRESQL_LIB})

#target_link_libraries(db_api_example ${POSTGRESQL_LIB})
