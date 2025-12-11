cmake_minimum_required(VERSION 3.15)

    project(TPC LANGUAGES CXX)
        
    # Set CXX standard
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
        
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
        
    # Include directories
    include_directories(include)
        
    # Add executable
    add_executable(TPC 
            src/main.cpp
    )
        
    # Compiler-specific options
    if(MSVC)
        target_compile_options(TPC PRIVATE /W4 /fsanitize=address)
    else()
        target_compile_options(TPC PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address)
    endif()
        
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(TPC PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(TPC PRIVATE -g -O0)
        endif()
    endif()
        
    # Enable testing
    enable_testing()
        
    # Install target
    #install(TARGETS TPC RUNTIME DESTINATION bin)
    