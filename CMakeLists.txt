cmake_minimum_required(VERSION 3.16)
project(CollisionDebugger) # Enter your project name here

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

# ============================================================================
# External Dependencies Configuration
# ============================================================================
# Set which dependencies to enable
set(DEP_GLFW ON "Build GLFW")
set(DEP_GLM ON "Build GLM")
set(DEP_GLEW ON "Build GLEW")
set(DEP_FREETYPE ON "Build FreeType")
set(DEP_STB ON "Include STB headers")
set(DEP_IMGUI ON "Build Dear ImGui")
set(DEP_ASSIMP ON "Build ASSIMP")
set(DEP_IMGUIZMO ON)

# ImGui options
set(DEP_IMGUI_BACKEND_GLFW ON "ImGui GLFW backend")
set(DEP_IMGUI_BACKEND_OPENGL3 ON "ImGui OpenGL3 backend")
set(DEP_IMGUI_FREETYPE ON "ImGui FreeType support")

add_subdirectory(dependencies External EXCLUDE_FROM_ALL)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE glew_static)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE stb)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        <glm/glm.hpp>
        <glm/ext.hpp>
)

# Defines PROJECT_ROOT macro for use in source files
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${PROJECT_SOURCE_DIR}")
