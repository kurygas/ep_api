add_library(Utility STATIC 
    random_functions.cpp
    request.cpp)
target_include_directories(Utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
