add_library(
    src STATIC 
    async_request.cpp
    cf_puller.cpp
    coroutine_task.cpp
    message_queue.cpp
    service.cpp
    storage.cpp
    utility_functions.cpp
)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
