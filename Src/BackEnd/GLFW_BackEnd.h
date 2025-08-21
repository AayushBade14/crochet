#pragma once

#include <GLFW/glfw3.h>

namespace GLFWBackEnd {
  bool Init(); // method to initialize glfw
  void Destroy(); // cleanup
  void MakeContextCurrent(); // sets the current fbo to the window

  // getters
  void* GetWindowPointer();
  int GetWindowWidth();
  int GetWindowHeight();
  
  // booleans
  bool WindowShouldClose();
}
