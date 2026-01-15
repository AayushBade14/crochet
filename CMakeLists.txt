cmake_minimum_required(VERSION 3.10)
project(
  crochet
  VERSION 1.0
  DESCRIPTION "Crochet-Engine"
  LANGUAGES C CXX
)

#Include Directories
include_directories(
  Vendor/glad/include
  Vendor/glm
  Vendor/stb
)

#Fetch GLFW
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9 
)
FetchContent_MakeAvailable(glfw)

#Source Files
file(GLOB_RECURSE CORE_SRC Core/*.cpp Core/*.h)
file(GLOB_RECURSE PLATFORM_SRC Platform/*.cpp Platform/*.h)
file(GLOB_RECURSE APP_SRC App/*.cpp App/*.h)
file(GLOB_RECURSE VENDOR_SRC Vendor/glad/src/*.c)

#Executable
add_executable(
  crochet
  ${CORE_SRC}
  ${PLATFORM_SRC}
  ${APP_SRC}
  ${VENDOR_SRC}
  main.cpp
)

#Link Libraries
target_link_libraries(
  crochet
  glfw
)
