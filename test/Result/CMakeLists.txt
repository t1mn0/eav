include(GoogleTest)

add_executable(result_tests
    Unit.cpp
    Func.cpp
)

target_link_libraries(result_tests
    PRIVATE
        eav
        gtest_main
)

target_include_directories(result_tests PRIVATE Result)

gtest_discover_tests(result_tests)
