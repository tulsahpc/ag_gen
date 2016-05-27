cmake_minimum_required(VERSION 2.8)

project(${PROJECT_NAME} C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

#find_package(FLEX)
#find_package(BISON)
#
#FLEX_TARGET(ExploitLexer exploit.l ${CMAKE_CURRENT_BINARY_DIR}/exploit.c
#        COMPILE_FLAGS "-P 'ep'")
#BISON_TARGET(ExploitParser exploit_parser.y ${CMAKE_CURRENT_BINARY_DIR}/exploit_parser.tab.c
#        COMPILE_FLAGS "-d -p 'ep'")
#ADD_FLEX_BISON_DEPENDENCY(ExploitLexer ExploitParser)
#
#FLEX_TARGET(NetworkLexer network_state.l ${CMAKE_CURRENT_BINARY_DIR}/network_state.c
#        COMPILE_FLAGS "-P 'ns'")
#BISON_TARGET(NetworkParser network_state_parser.y ${CMAKE_CURRENT_BINARY_DIR}/network_state_parser.tab.c
#        COMPILE_FLAGS " -d -p 'ns'")
#ADD_FLEX_BISON_DEPENDENCY(NetworkLexer NetworkParser)

include_directories("./")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#caggen.c
#identifier.c
#ag_generator_utils.c
#exploit_utils.c
#network_state_util.c
#${BISON_ExploitParser_OUTPUTS}
#${FLEX_ExploitLexer_OUTPUTS}
#${BISON_NetworkParser_OUTPUTS}
#${FLEX_NetworkLexer_OUTPUTS}
add_executable(attack-graph-generator
        ag_network.c
        ag_asset.c
        db_util.c)
target_link_libraries(attack-graph-generator)

find_package(PostgreSQL)
add_executable(db_test
        db_test.c
        ag_asset.c
        ag_network.c
        db_util.c)
target_link_libraries(db_test
        ${PostgreSQL_LIBRARIES})
