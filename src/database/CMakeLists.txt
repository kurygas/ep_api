add_library(Database STATIC 
    group.cpp
    session.cpp
    test_result.cpp
    test_variant.cpp
    test.cpp
    user.cpp)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)