#pragma once

#include "./Chunk.h"

struct GLFWwindow;

const int WORLD_SIZE = 200;

struct World
{
  std::vector<Chunk> m_Chunks; 

  void Init();
  void Render(GLFWwindow* window, float mX, float mY, float cx, float cy, float z, float aspect);
  bool IsChunkVisible(Chunk& chunk, float cx, float cy, float z, float aspect);
};
