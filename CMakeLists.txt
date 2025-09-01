cmake_minimum_required(VERSION 3.15)
project(throwless
    LANGUAGES CXX
    VERSION 1.0.0
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================
# Options and configuration
# =============================================
option(BUILD_TESTS "Build tests" ON)

# =============================================
# Compiler setup
# =============================================
# ?

# =============================================
# Library
# =============================================
# ?

# =============================================
# Installation
# =============================================
# ?

# =============================================
# Config files for find_package
# =============================================
# ?

# =============================================
# Test
# =============================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
