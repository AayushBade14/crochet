//-------------------------------------------------------------------------
//  Name: GLFWContext.cpp
//
//  Desc: An implementation of GLFWContext class
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#include "./GLFWContext.h"

#include <GLFW/glfw3.h>

namespace Crochet
{
  GLFWContext::GLFWContext()
  {
    if(m_RefCount++ == 0)
    {
      glfwInit();
    }
  }

  GLFWContext::~GLFWContext()
  {
    if(m_RefCount-- == 0)
    {
      glfwTerminate(); 
    }
  }
}
