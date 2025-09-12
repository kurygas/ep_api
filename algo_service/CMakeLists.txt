cmake_minimum_required(VERSION 3.20)
project(algo_service)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpr REQUIRED)
find_package(amqpcpp REQUIRED)
add_executable(algo_service src/main.cpp)
add_subdirectory(src)
target_link_libraries(
    algo_service PUBLIC
    src
    amqpcpp
    cpr::cpr
    OpenSSL::SSL
    OpenSSL::Crypto
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
)