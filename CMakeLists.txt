cmake_minimum_required(VERSION 3.5)
project(ag_gen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall -DDEBUG_BUILD")

file(GLOB ag_gen_src
        "src/*.cpp"
        "src/*.h")

add_executable(ag_gen ${ag_gen_src})
add_executable(sha256_test "tools/sha256_test.cpp")
add_executable(graphing_test "tools/graphing_db.cpp")

include_directories(
    "/usr/local/include"
    "/usr/include/postgresql"
)

if(APPLE)
    include_directories("/usr/local/opt/openssl/include")
endif()

find_library(HIREDIS_LIB hiredis /usr/local/lib)
find_library(POSTGRESQL_LIB pq /usr/local/lib)
target_link_libraries(ag_gen ${HIREDIS_LIB} ${POSTGRESQL_LIB})

find_library(OPENSSL_LIB ssl /usr/local/opt/openssl/lib)
find_library(OPENSSL_CRYPTO_LIB crypto /usr/local/opt/openssl/lib)
target_link_libraries(sha256_test ${OPENSSL_LIB} ${OPENSSL_CRYPTO_LIB})

target_link_libraries(graphing_test ${POSTGRESQL_LIB})
