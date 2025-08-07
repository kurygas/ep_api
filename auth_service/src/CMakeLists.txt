add_library(
    src STATIC 
    auth_service.cpp
    crypto.cpp
)
target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
