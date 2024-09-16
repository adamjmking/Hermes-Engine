cmake_minimum_required(VERSION 3.10)

project(HermesEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Set include and library directories
include_directories(
    include                     # Your project's include directory
    ${CMAKE_SOURCE_DIR}/lib     # Directory where tiny_obj_loader.h is located
    ${Vulkan_INCLUDE_DIRS}      # Vulkan include directories
)


link_directories(${CMAKE_SOURCE_DIR}/lib)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src directory.")
endif()

# Create the executable
add_executable(HermesEngine ${SOURCES})

# Link libraries
target_link_libraries(HermesEngine glfw3 ${Vulkan_LIBRARIES})