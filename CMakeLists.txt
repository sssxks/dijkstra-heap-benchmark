# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(pathfinder VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the C++ standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get all .cpp files in the src/ directory
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Create a single executable from all .cpp files
add_executable(pathfinder ${SRC_FILES})

# Set compile options for the executable
target_compile_options(pathfinder PRIVATE 
    -Wall -Wextra -Weffc++
    -Werror=uninitialized
    -Werror=return-type
    -Wconversion -Wsign-compare
    -Werror=unused-result
    -Werror=suggest-override
    -Wzero-as-null-pointer-constant
    # -Wmissing-declarations
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Werror
)

# Include header files from include/ directory
target_include_directories(pathfinder PRIVATE include)

# enable_testing()

# Add the test using the custom script
# add_test(NAME Testpathfinder
#          COMMAND ${CMAKE_SOURCE_DIR}/script/run_test.sh
#          $<TARGET_FILE:pathfinder> ${CMAKE_SOURCE_DIR}/input.txt ${CMAKE_SOURCE_DIR}/output.txt)
