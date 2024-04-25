cmake_minimum_required(VERSION 3.25)
project(Project_Steganography)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(Project_Steganography main.cpp)
target_link_libraries(Project_Steganography fmt)