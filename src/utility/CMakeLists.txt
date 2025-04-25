add_library(Utility STATIC 
    json.cpp
    request.cpp)
target_include_directories(Utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})