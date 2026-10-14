# Emulator core
add_library(i8080_core
    src/cpu.cpp
    src/memory.cpp
    src/rom_loader.cpp
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
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.8.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

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
