cmake_minimum_required(VERSION 3.20)

project(Intel8080Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Unit tests
include(CTest)

add_subdirectory(i8080)
