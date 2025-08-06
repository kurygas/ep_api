add_library(
    Services STATIC
    cf_service.cpp
    atc_service.cpp
)
target_include_directories(Services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)
target_include_directories(Services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../database)
