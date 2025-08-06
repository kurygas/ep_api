add_library(
    Src STATIC 
    auth_service.cpp
    crypto.cpp
)
target_include_directories(Src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
