cmake_minimum_required(VERSION 3.22)
project(ep_api)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Wt REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(amqpcpp REQUIRED)
add_subdirectory(src)
add_executable(ep_api src/main.cpp)
target_link_libraries(
    ep_api PUBLIC
    Server
    Resources
    MessageQueue
    Database
    Utility
    amqpcpp
    jwt-cpp::jwt-cpp
    Wt::Wt
    Wt::HTTP
    Wt::Dbo
    Wt::DboSqlite3
    OpenSSL::SSL
    OpenSSL::Crypto
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
)
