add_library(
    MessageQueue STATIC
    message_queue.cpp
)
target_include_directories(MessageQueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MessageQueue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../database)
