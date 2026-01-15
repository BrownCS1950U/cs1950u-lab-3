# CMakeLists.txt for ImGuizmo
# https://github.com/CedricGuillemet/ImGuizmo
#
# Usage: add_subdirectory(path/to/ImGuizmo) and link against "ImGuizmo"
# Requires: ImGui to be available as a target

cmake_minimum_required(VERSION 3.16)
project(ImGuizmo LANGUAGES CXX)

# ============================================================================
# Validation
# ============================================================================

# Verify ImGui target exists
if(NOT TARGET imgui)
    message(FATAL_ERROR
            "ImGuizmo requires ImGui.\n"
            "Please ensure 'imgui' target is available before adding ImGuizmo."
    )
endif()

# ============================================================================
# Create the ImGuizmo library
# ============================================================================

add_library(ImGuizmo STATIC)

# Add alias for consistency
add_library(ImGuizmo::ImGuizmo ALIAS ImGuizmo)

# Core source files
target_sources(ImGuizmo PRIVATE
        ImGuizmo.cpp
        ImGuizmo.h
)

# Optional extensions (can be made optional via CMake options if desired)
option(IMGUIZMO_INCLUDE_CURVE_EDIT "Include ImCurveEdit" ON)
option(IMGUIZMO_INCLUDE_GRADIENT "Include ImGradient" ON)
option(IMGUIZMO_INCLUDE_SEQUENCER "Include ImSequencer" ON)
option(IMGUIZMO_INCLUDE_GRAPH_EDITOR "Include GraphEditor" ON)

if(IMGUIZMO_INCLUDE_CURVE_EDIT)
    target_sources(ImGuizmo PRIVATE
            ImCurveEdit.cpp
            ImCurveEdit.h
    )
endif()

if(IMGUIZMO_INCLUDE_GRADIENT)
    target_sources(ImGuizmo PRIVATE
            ImGradient.cpp
            ImGradient.h
    )
endif()

if(IMGUIZMO_INCLUDE_SEQUENCER)
    target_sources(ImGuizmo PRIVATE
            ImSequencer.cpp
            ImSequencer.h
    )
endif()

if(IMGUIZMO_INCLUDE_GRAPH_EDITOR)
    target_sources(ImGuizmo PRIVATE
            GraphEditor.cpp
            GraphEditor.h
    )
endif()

# Additional header-only utilities
target_sources(ImGuizmo PRIVATE
        ImZoomSlider.h
        ImLightRig.h
)

# Set include directories
target_include_directories(ImGuizmo PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ImGuizmo>
)

# Link to ImGui
target_link_libraries(ImGuizmo PUBLIC imgui)

# Use the same C++ standard as ImGui (C++20)
target_compile_features(ImGuizmo PUBLIC cxx_std_20)

# ============================================================================
# Installation (optional, disabled by default)
# ============================================================================

option(IMGUIZMO_INSTALL "Generate installation target" OFF)

if(IMGUIZMO_INSTALL)
    include(GNUInstallDirs)

    install(TARGETS ImGuizmo
            EXPORT ImGuizmoTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(FILES
            ImGuizmo.h
            ImCurveEdit.h
            ImGradient.h
            ImSequencer.h
            GraphEditor.h
            ImZoomSlider.h
            ImLightRig.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ImGuizmo
    )

    install(EXPORT ImGuizmoTargets
            FILE ImGuizmoTargets.cmake
            NAMESPACE ImGuizmo::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ImGuizmo
    )
endif()

# Print configuration summary
message(STATUS "ImGuizmo Configuration:")
message(STATUS "  C++ Standard: C++20")
message(STATUS "  Core: ImGuizmo")
message(STATUS "  Extensions:")
message(STATUS "    - ImCurveEdit:   ${IMGUIZMO_INCLUDE_CURVE_EDIT}")
message(STATUS "    - ImGradient:    ${IMGUIZMO_INCLUDE_GRADIENT}")
message(STATUS "    - ImSequencer:   ${IMGUIZMO_INCLUDE_SEQUENCER}")
message(STATUS "    - GraphEditor:   ${IMGUIZMO_INCLUDE_GRAPH_EDITOR}")

