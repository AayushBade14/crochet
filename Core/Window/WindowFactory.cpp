//-------------------------------------------------------------------------
//  Name: WindowFactory.cpp
//
//  Desc: An implementation of the WindowFactory class - for window creation
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#include "./WindowFactory.h"

#include "../../Platform/GLFW/GLFWWindow.h"

namespace Crochet
{
  std::unique_ptr<Window> WindowFactory::Create(WindowBackend backend, const WindowProp& prop)
  {
    switch(backend)
    {
      case WindowBackend::GLFW :
        return std::make_unique<GLFWWindow>(prop);

      return nullptr;
    }
  }
}

