#pragma once

#include "./World.h"

struct Editor
{
  World m_World;

  Editor(){m_World.Init();}

  ~Editor(){}
};
