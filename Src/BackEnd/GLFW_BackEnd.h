#pragma once

#include <GLFW/glfw3.h>

namespace GLFWBackEnd {
  bool Init(); // method to initialize glfw
  void Destroy(); // cleanup
  
  // getters
  void* GetWindowPointer();
  int GetWindowWidth();
  int GetWindowHeight();
  
  // booleans
  bool WindowShouldClose();
}
