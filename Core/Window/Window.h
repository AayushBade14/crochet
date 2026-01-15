//-------------------------------------------------------------------------
//  Name: Window.h
//
//  Desc: An interface for the window-class
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

namespace Crochet
{
  class Window
  {
    public:

      virtual ~Window() = default;
      
      virtual void* GetNativeHandle() const = 0;
      virtual bool ShouldClose() const = 0;
      virtual int GetWidth() const = 0;
      virtual int GetHeight() const = 0;

      virtual void PollEvents() = 0;
      virtual void SwapBuffers() = 0;
  };
}

