cmake_minimum_required(VERSION 3.15)
    project(tpc LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    # Include directories
    include_directories(include)
    
    # Add executable
    add_executable(tpc 
        src/main.c
        src/asmgen.c
    )
    
    # Compiler-specific options
    if(MSVC)
        target_compile_options(tpc PRIVATE /W4)
    else()
        target_compile_options(tpc PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(tpc PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(tpc PRIVATE -g -O0)
        endif()
    endif()
    
    # Enable testing
    enable_testing()
    
    # Install target
    #install(TARGETS tpc  RUNTIME DESTINATION bin)
        