add_library(
    Resources STATIC
    group_resource.cpp
    problem_resource.cpp
    user_resource.cpp
    work_resource.cpp
    work_result_resource.cpp
    root_requirements.cpp
    get_solution_resource.cpp
    point_resource.cpp
    semester_resource.cpp
    semester_result_resource.cpp
    solutions.cpp
)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../database)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../services)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../message_queue)
