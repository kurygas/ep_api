cmake_minimum_required(VERSION 3.30)
project(ep_api)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Wt REQUIRED Wt HTTP Dbo DboSqlite3)
find_package(OpenSSL REQUIRED)
find_package(cpr REQUIRED)
find_package(Boost REQUIRED)
add_subdirectory(src)
add_executable(ep_api main.cpp)
target_include_directories(ep_api PUBLIC ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})
target_link_libraries(
    ep_api PUBLIC
    Server
    Resources
    Services
    Database
    Utility
    cpr::cpr
    Wt::Wt
    Wt::HTTP
    Wt::Dbo
    Wt::DboSqlite3
    OpenSSL::SSL
    OpenSSL::Crypto
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
)
