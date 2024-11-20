cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Copy sample data to the build directory
file(COPY sample_data/ DESTINATION sample_data/)

# Enable verbose makefile output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set compiler warnings based on the compiler type
if(MSVC)
    # MSVC specific flags
    add_compile_options(/W4)  # Use /W4 for MSVC warnings
else()
    # For GCC or Clang compilers
    add_compile_options(-Wall -Wextra -pedantic)  # Use these flags for GCC/Clang
endif()

# Create executables
add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch main.cpp)

# Link rapidjson to the rapidJSONExample target
target_include_directories(rapidJSONExample PRIVATE rapidjson/)

# Disable verbose makefile output
set(CMAKE_VERBOSE_MAKEFILE OFF)
