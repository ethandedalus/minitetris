cmake_minimum_required(VERSION 3.29)
project(minitetris LANGUAGES C CXX OBJC)

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
include(cmake/fetch_raylib.cmake)

set(
    SRCS

    src/Tetris.cc
    src/Timing.cc
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cc ${SRCS})

target_link_libraries(${PROJECT_NAME} raylib)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s EXPORTED_RUNTIME_METHODS=['requestFullscreen'] -s SINGLE_FILE=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
endif ()

if(PLATFORM STREQUAL "Web")
    # Define output directory (adjust this if your output is elsewhere)
    set(WEB_OUTPUT_FILES ${PROJECT_NAME}.js)

    # Compute source and destination directories
    set(SOURCE_DIR "${CMAKE_BINARY_DIR}")
    set(DEST_DIR "${CMAKE_SOURCE_DIR}/web/resources")

    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WEB=1)

    # Create custom target to copy the files after build
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${WEB_OUTPUT_FILES}
                "${DEST_DIR}"
        WORKING_DIRECTORY "${SOURCE_DIR}"
        COMMENT "Copying Web output files to web/resources"
    )
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework GLUT")
endif()

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running app"
)
