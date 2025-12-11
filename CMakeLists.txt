cmake_minimum_required(VERSION 3.10)
project(
  crochet
  VERSION 1.0
  DESCRIPTION "crochet game-engine"
  LANGUAGES C CXX
)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_subdirectory(glm)

add_library(
  glad
  STATIC
  ./glad/src/glad.c
)
target_include_directories(
  glad
  PUBLIC
  ./glad/include
)

add_executable(
  crochet
  main.cpp
)
target_link_libraries(
  crochet
  PUBLIC
  glad
  OpenGL::GL
  glfw
)
