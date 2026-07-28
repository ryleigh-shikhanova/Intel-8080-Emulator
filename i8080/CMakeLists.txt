cmake_minimum_required(VERSION 3.20)

project(Intel8080Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Emulator core
add_library(i8080_core
    src/cpu.cpp
    src/memory.cpp
)

target_include_directories(i8080_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Main emulator executable
add_executable(i8080
    src/main.cpp
)

target_link_libraries(i8080
    PRIVATE
        i8080_core
)

# Unit tests
include(CTest)

if(BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)

    add_executable(i8080_tests
        tests/arithmetic_tests.cpp
        tests/cpu_state_tests.cpp
	tests/data_transfer_tests.cpp
    )

    target_link_libraries(i8080_tests
        PRIVATE
            i8080_core
            Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(i8080_tests)
endif()
