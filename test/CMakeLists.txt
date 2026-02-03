cmake_minimum_required(VERSION 3.20)
project(eav_tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

FetchContent_MakeAvailable(
  googletest
)

add_subdirectory(Result)
add_subdirectory(Option)
