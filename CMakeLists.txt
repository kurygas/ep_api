cmake_minimum_required(VERSION 3.10)
project(ep_api)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_program(PYTEST NAMES pytest pytest3)
if(NOT PYTEST)
    message(WARNING "pytest not found")
else()
    add_test(
        NAME python_tests
        COMMAND ${PYTEST} ${CMAKE_CURRENT_SOURCE_DIR}/tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

find_package(Wt REQUIRED Wt HTTP Dbo DboSqlite3)
add_subdirectory(src)
add_executable(ep_api main.cpp)
target_link_libraries(
    ep_api PUBLIC
    Server
    Resources
    Database
    Utility
    Wt::Wt 
    Wt::HTTP 
    Wt::Dbo 
    Wt::DboSqlite3 
)
