cmake_minimum_required(VERSION 3.20)

project(GrassMania)

add_subdirectory("dependencies/glfw")
add_subdirectory("dependencies/glm")
add_subdirectory("dependencies/gl3w")

add_executable(GrassMania src/main.cpp)

target_link_libraries(GrassMania PUBLIC glfw glm gl3w)