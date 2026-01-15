//-------------------------------------------------------------------------
//  Name: Application.cpp
//
//  Desc: Implementation for Application
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#include "./Application.h"

#include "../Core/Window/WindowFactory.h"

namespace Crochet
{
  Application::Run()
  {
    WindowProp prop;
    prop.Width = 1280;
    prop.Height = 720;
    prop.Title = "Window-Test";

    auto window = WindowFactory::Create(WindowBackend::GLFW, prop);

    while(!window->ShouldClose())
    {
      window->PollEvents();

      window->SwapBuffers();
    }
  }
}
