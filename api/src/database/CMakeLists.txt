add_library(
    Database STATIC
    validator.cpp
    group.cpp
    problem.cpp
    session.cpp
    user.cpp
    work_result.cpp
    work.cpp
    point.cpp
    semester.cpp
    semester_result.cpp
)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)
