cmake_minimum_required(VERSION 3.20)

project(GrassMania)

add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")

# Automatically add all files from the src directory
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.*)

add_executable(GrassMania ${MY_SOURCES})

target_link_libraries(GrassMania PUBLIC glfw glm gl3w)

add_custom_target(copy_resources ALL
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				${PROJECT_SOURCE_DIR}/resources
				${PROJECT_BINARY_DIR}/resources)

add_dependencies(GrassMania copy_resources)