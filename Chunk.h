#pragma once

#include "./Tile.h"
#include <vector>

const int CHUNK_SIZE = 32;

struct GLFWwindow;

struct Chunk
{
  float m_XPos;
  float m_YPos;

  Tile m_Tiles[CHUNK_SIZE][CHUNK_SIZE];
  
  std::vector<float> m_VertexData;
  
  unsigned int m_Vbo;
  unsigned int m_Vao;
  
  void InitChunk();

  void push(float x, float y, float u, float v);
  
  void BakeVertices();

  void RenderChunk();

  bool Collide(int i, int j, float xworld, float yworld);
  
  void Update(GLFWwindow* window, float xworld, float yworld);
};

