//-------------------------------------------------------------------------
//  Name: GLFWWindow.h
//
//  Desc: An implementation of Window-interface for GLFW-backend
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

#include "../../Core/Window/Window.h"
#include "../../Core/Window/WindowProp.h"

//forward declaring
struct GLFWwindow;
class GLFWContext;

namespace Crochet
{
  class GLFWWindow
  {
    private:
      
      GLFWwindow* m_Window = nullptr;
      WindowProp m_WindowProp;
      GLFWContext m_Context;

    public:
      
      GLFWWindow(const WindowProp& prop);
      
      ~GLFWWindow() override;

      void* GetNativeHandle() const override;
      
      bool ShouldClose() const override;

      int GetWidth() const override;
      int GetHeight() const override;

      void PollEvents() override;

      void SwapBuffers() override;
  };
}


