cmake_minimum_required(VERSION 3.12)
project(fpp_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
endif()

add_executable(ArithmeticTestOption ArithmeticTestOption.cpp)
add_executable(GeneralTestOption GeneralTestOption.cpp)

target_link_libraries(
    ArithmeticTestOption
    PRIVATE fpp
    PRIVATE GTest::gtest GTest::gtest_main
)

target_link_libraries(
    GeneralTestOption
    PRIVATE fpp
    PRIVATE GTest::gtest GTest::gtest_main
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(GeneralTestOption PRIVATE -fsanitize=address,undefined)
    target_link_options(GeneralTestOption PRIVATE -fsanitize=address,undefined)
    target_compile_options(ArithmeticTestOption PRIVATE -fsanitize=address,undefined)
    target_link_options(ArithmeticTestOption PRIVATE -fsanitize=address,undefined)
endif()

add_test(NAME ArithmeticTestOption COMMAND ArithmeticTestOption)
add_test(NAME GeneralTestOption COMMAND GeneralTestOption)