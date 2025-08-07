cmake_minimum_required(VERSION 3.20)
project(auth_service)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(Crow REQUIRED)
add_executable(auth_service src/main.cpp)
add_subdirectory(src)
target_link_libraries(
    auth_service PUBLIC
    src
    Crow::Crow
    redis++::redis++
    hiredis::hiredis
    jwt-cpp::jwt-cpp
    OpenSSL::SSL
    OpenSSL::Crypto
)
