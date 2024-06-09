cmake_minimum_required(VERSION 3.25)
project(Project_Steganography)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(SFML)

set(SOURCE_FILES
        main.cpp
        functions.hpp
        crypto.cpp
        info.cpp
        other.cpp
        decrypto.cpp
)

add_executable(Project_Steganography
        main.cpp
        functions.hpp
        crypto.cpp
        info.cpp
        other.cpp
        decrypto.cpp
)

target_link_libraries(Project_Steganography fmt sfml-graphics)