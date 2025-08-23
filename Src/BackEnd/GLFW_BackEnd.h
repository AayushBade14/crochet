#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

namespace GLFWBackEnd {
  bool Init(); // method to initialize glfw
  void Destroy(); // cleanup
  void MakeContextCurrent(); // sets the current fbo to the window

  // getters
  void* GetWindowPointer();
  int GetWindowWidth();
  int GetWindowHeight();
  float GetDt();

  // booleans
  bool WindowShouldClose();

  // timer
  void UpdateTimer();

  void SwapBuffers();
  void PollEvents();
}
