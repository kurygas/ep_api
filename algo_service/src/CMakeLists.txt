add_library(
    src STATIC 
    service.cpp
    message_queue.cpp
)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
