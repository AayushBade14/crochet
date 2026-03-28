#pragma once

#include "./Chunk.h"

struct GLFWwindow;

const int WORLD_SIZE = 30;

struct World
{
  std::vector<Chunk> m_Chunks; 

  void Init();
  void Render(GLFWwindow* window, float mX, float mY, float cx, float cy, float z, float aspect, bool uifreE);
  void AddChunk(float x, float y);
  void EraseChunk(float x, float y);
  //bool IsChunkVisible(Chunk& chunk, float cx, float cy, float z, float aspect);
};
