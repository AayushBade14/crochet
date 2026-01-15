//-------------------------------------------------------------------------
//  Name: WindowProp.h
//
//  Desc: A structure for holding window-specific properties
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

#include <string>

namespace Crochet
{
  struct WindowProp
  {
    int Width = 1280;
    int Height = 720; 
    std::string Title = "Crochet-Engine";
  };
}

