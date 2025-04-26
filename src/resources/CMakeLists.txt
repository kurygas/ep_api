add_library(Resources STATIC
    group_resources.cpp
    resource.cpp
    user_resources.cpp)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)
target_include_directories(Resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../database)
