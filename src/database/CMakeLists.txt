add_library(Database STATIC 
    checker.cpp
    group.cpp
    problem.cpp
    session.cpp
    user.cpp
    work.cpp)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})