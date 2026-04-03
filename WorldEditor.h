#pragma once

#include "./World.h"

struct WorldEditor
{
  World m_WorldMap;

  void Init();
  void Render();
  void Update();
};
