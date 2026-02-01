cmake_minimum_required(VERSION 3.20)
project(eav LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(eav INTERFACE)

target_include_directories(eav INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(eav INTERFACE cxx_std_20)

enable_testing()
add_subdirectory(test)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE eav)
