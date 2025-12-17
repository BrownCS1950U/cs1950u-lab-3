file (GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable (${PROJECT_NAME} "${SOURCE_DIR}/main.cpp")
target_sources(
        ${PROJECT_NAME} PRIVATE
        ${SOURCE_FILES}
)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/collision")