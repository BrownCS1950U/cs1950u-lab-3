file (GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

message(STATUS "Source files in collision module: ${SOURCE_FILES}")
target_sources(
        ${PROJECT_NAME} PRIVATE
        ${SOURCE_FILES}
)