cmake_minimum_required(VERSION 3.10)
project(
  crochet2D
  DESCRIPTION "crochet2D: A 2D top down game engine"
  VERSION 1.0
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

add_subdirectory(./Vendor/glm)

add_library(
  glad
  STATIC
  ./Vendor/glad/src/glad.c
)
target_include_directories(
  glad
  PUBLIC
  ./Vendor/glad/include
)

add_library(
  chunk
  STATIC
  ./Chunk.cpp
)
target_include_directories(
  chunk
  PUBLIC
  ./
  ./Vendor/glad/include
)

add_library(
  world
  STATIC
  ./World.cpp
)
target_include_directories(
  world
  PUBLIC
  ./
  ./Vendor/glad/include
)
target_link_libraries(
  world
  PUBLIC
  chunk
)

add_library(
  stb_image
  STATIC
  ./Vendor/stb_image/stb_image.cpp
)
target_include_directories(
  stb_image
  PUBLIC
  ./Vendor/stb_image
)

add_executable(
  crochet2D
  main.cpp
)
target_link_libraries(
  crochet2D
  PUBLIC
  glad
  glm::glm
  OpenGL::GL
  glfw
  stb_image
  chunk
  world
)
