//-------------------------------------------------------------------------
//  Name: GLFWWindow.cpp
//
//  Desc: An implementation of GLFWWindow class
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

#include "./GLFWWindow.h"

#include "./GLFWContext.h"

#include <GLFW/glfw3.h>

namespace Crochet
{
  GLFWWindow::GLFWWindow(const WindowProp& prop) :  GLFWContext()
  {
    m_WindowProp.Width = prop.Width;
    m_WindowProp.Height = prop.Height;
    m_WindowProp.Title = prop.Title;

    //TO-DO: Add rendering api based window-hints
    
    m_Window = glfwCreateWindow(
      m_WindowProp.Width,
      m_WindowProp.Height,
      m_WindowProp.Title.c_str(),
      nullptr,
      nullptr
    );

    glfwMakeContextCurrent(m_Window);
  }

  GLFWWindow::~GLFWWindow() override
  {
    glfwDestroyWindow(m_Window);
  }

  void* GLFWWindow::GetNativeHandle() const override
  {
    return static_cast<void*>(m_Window);
  }

  bool GLFWWindow::ShouldClose() const override
  {
    return glfwWindowShouldClose(m_Window);
  }

  int GLFWWindow::GetWidth() const override
  {
    return m_WindowProp.Width;
  }

  int GLFWWindow::GetHeight() const override
  {
    return m_WindowProp.Height;
  }

  void GLFWWindow::PollEvents() override
  {
    glfwPollEvents();
  }

  void GLFFWWindow::SwapBuffers() override
  {
    glfwSwapBuffers(m_Window);
  }
}
