cmake_minimum_required(VERSION 3.20)

project(GrassMania)

add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")

# Automatically add all files from the src directory
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.*)

add_executable(GrassMania ${MY_SOURCES})

target_link_libraries(GrassMania PUBLIC glfw glm gl3w)

# Copy the resources folder to the build directory
add_custom_command(
    TARGET GrassMania POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:GrassMania>/resources
)
