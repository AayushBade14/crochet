cmake_minimum_required(VERSION 3.10)
project(
  crochet_engine
  VERSION 1.0
  DESCRIPTION "Crochet-Engine: Custom game engine made from scratch in OpenGL & C++"
  LANGUAGES C CXX
)

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG: 3.3.9
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
  ogl
  STATIC
  ./Include/OGL.cpp
)
target_include_directories(
  ogl
  PUBLIC
  ./
  ./Vendor/glad/include
  ./Vendor
)

add_library(
  stb
  STATIC
  ./Vendor/Stb/stb_image.cpp
)
target_include_directories(
  stb
  PUBLIC
  ./Vendor
)

add_executable(
  crochet_engine
  ./Main.cpp
)
target_link_libraries(
  crochet_engine
  PUBLIC
  OpenGL::GL
  glfw
  glm::glm
  glad
  ogl
  stb
)

