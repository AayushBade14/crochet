//-------------------------------------------------------------------------
//  Name: WindowFactory.h
//
//  Desc: A factory for creating window-instances
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

#include <memory>

#include "./Window.h"
#include "./WindowProp.h"
#include "./WindowBackend.h"

namespace Crochet
{
  class WindowFactory
  {
    public:

      static std::unique_ptr<Window> Create(WindowBackend backend, const WindowProp& prop);
  };
}

