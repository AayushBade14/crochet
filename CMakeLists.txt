cmake_minimum_required(VERSION 3.10)
project(
  crochet
  VERSION 1.0
  DESCRIPTION "A custom game engine written from scratch in OpenGL & C++"
  LANGUAGES C CXX
)

# find opengl
find_package(OpenGL REQUIRED)

# ----- FETCH-GLFW -----

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9
)

FetchContent_MakeAvailable(glfw)

# ----- LIBS -----

# glm

add_subdirectory(Vendor/glm)

# glad

add_library(
  glad
  STATIC
  Vendor/glad/src/glad.c
)

target_include_directories(
  glad
  PUBLIC
Vendor/glad/include
)

# stb_image

add_library(
  stb
  STATIC
  Vendor/Stb/stb_image.cpp
)

target_include_directories(
  stb
  PUBLIC
  Vendor
)

# glfw-backend

add_library(
  glfw_backend
  STATIC
  Src/BackEnd/GLFW_BackEnd.cpp
)

target_include_directories(
  glfw_backend
  PUBLIC
  Src
)

target_link_libraries(
  glfw_backend
  PUBLIC
  glfw
)

# ogl-backend

add_library(
  ogl_backend
  STATIC
  Src/API/OpenGL/BackEnd/GL_BackEnd.cpp
)

target_include_directories(
  ogl_backend
  PUBLIC
  Src
)

target_link_libraries(
  ogl_backend
  PUBLIC
  glfw_backend
  glad
  glm::glm
)

# renderer

add_library(
  renderer
  STATIC
  Src/Renderer/Renderer.cpp
)

target_include_directories(
  renderer
  PUBLIC
  Src
)

target_link_libraries(
  renderer
  PUBLIC
  ogl-backend
  glfw_backend
  glshader
  glvbo
  glvao
)

# ----- TYPES -----

# glshader

add_library(
  glshader
  STATIC
  Src/API/OpenGL/Types/GL_Shader.cpp
)

target_include_directories(
  glshader
  PUBLIC
  Src
)

target_link_libraries(
  glshader
  PUBLIC
  ogl_backend
  glm::glm
)

# glvbo

add_library(
  glvbo
  STATIC
  Src/API/OpenGL/Types/GL_VBO.cpp
)

target_include_directories(
  glvbo
  PUBLIC
  Src
)

target_link_libraries(
  glvbo
  PUBLIC
  ogl_backend
)

# glvao

add_library(
  glvao
  STATIC
  Src/API/OpenGL/Types/GL_VAO.cpp
)

target_include_directories(
  glvao
  PUBLIC
  Src
)

target_link_libraries(
  glvao
  PUBLIC
  ogl_backend
)

# ----- MAIN-EXE -----

add_executable(
  crochet
  Src/Main.cpp
)

target_link_libraries(
  crochet
  PUBLIC
  glad
  glfw
  OpenGL::GL
  glm::glm
  glshader
  glvbo
  glvao
  renderer
  ogl_backend
  glfw_backend
  stb
)
