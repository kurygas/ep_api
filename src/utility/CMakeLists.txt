add_library(
    Utility STATIC 
    http_exceptions.cpp
    random_functions.cpp
    request.cpp
    utility_functions.cpp
)
target_include_directories(Utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
