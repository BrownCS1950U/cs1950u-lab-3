cmake_minimum_required(VERSION 3.16)
project(CollisionDebugger) # Enter your project name here

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

# ============================================================================
# External Dependencies Configuration
# ============================================================================
# Set which dependencies to enable
set(DEP_ENABLE_ALL ON)

# Path to shared dependencies directory (in parent folder)
set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies")
add_subdirectory(${DEPENDENCIES_DIR} External EXCLUDE_FROM_ALL)


# Link all dependencies using the unified interface library
target_link_libraries(${PROJECT_NAME} PRIVATE
        Dependencies::AllEnabled
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        <glm/glm.hpp>
        <glm/ext.hpp>
)

# Defines PROJECT_ROOT macro for use in source files
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${PROJECT_SOURCE_DIR}")
