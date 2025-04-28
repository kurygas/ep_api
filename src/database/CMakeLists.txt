add_library(Database STATIC 
    checker.cpp
    group.cpp
    problem.cpp
    session.cpp
    subject_type.cpp
    user.cpp
    work_result.cpp
    work.cpp)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)
