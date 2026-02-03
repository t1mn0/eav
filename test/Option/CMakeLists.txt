include(GoogleTest)

add_executable(option_tests
    Unit.cpp
    Func.cpp
)

target_link_libraries(option_tests
    PRIVATE
        eav
        gtest_main
)

target_include_directories(option_tests PRIVATE Result)

gtest_discover_tests(option_tests)
