add_library(Database STATIC 
    group.cpp
    group.h
    session.cpp
    session.h
    test_result.cpp
    test_result.h
    test_variant.cpp
    test_variant.h
    test.cpp
    test.h
    user.cpp
    user.h)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../utility)