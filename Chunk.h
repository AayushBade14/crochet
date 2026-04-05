#pragma once

#include "./Tile.h"
#include <vector>

const int BASE_LAYER_COUNT = 3;
const int DECORATION_LAYER_COUNT = 2;

const int CHUNK_SIZE = 32;
const float ATLAS_SIZE = 1024.0f;

const int NUM_LAYERS = 5;

//const int LAYER_OFFSET = 1024 * 6 * 4;
const int VERTEX_COUNT_PER_LAYER = 1024 * 6;

struct GLFWwindow;

struct ChunkCoords
{
  int m_X;
  int m_Y;

  ChunkCoords& operator=(const ChunkCoords& other)
  {
    if(this == &other) return *this;

    m_X = other.m_X;
    m_Y = other.m_Y;

    return *this;
  }

  bool operator==(const ChunkCoords& other) const
  {
    return m_X == other.m_X && m_Y == other.m_Y;
  }
};

namespace std
{
  template <>
  struct hash<ChunkCoords>
  {
    size_t operator()(const ChunkCoords& cc) const
    {
      size_t h1 = std::hash<float>{}(cc.m_X);
      size_t h2 = std::hash<float>{}(cc.m_Y);

      return h1 ^ (h2 << 1);
    }
  };
}

struct TileLayer
{
  //bool m_IsEditorVisible = true;
  //bool m_IsRuntimeVisible = true;
  //bool m_IsLocked = false;
  Tile m_Tiles[CHUNK_SIZE][CHUNK_SIZE];
};

/*
struct CollisionLayer
{
  bool m_IsVisible = true;
  bool m_IsLocked = false;
  uint8_t m_Tiles[CHUNK_SIZE][CHUNK_SIZE];
};
*/

struct Chunk
{
  ChunkCoords m_ChunkCoords;

  //Tile m_Tiles[CHUNK_SIZE][CHUNK_SIZE];
  // 0 - base_layer0
  // 1 - base_layer1
  // 2 - base_layer2
  // 3 - decoration_layer0
  // 4 - decoration_layer1

  TileLayer m_Layers[NUM_LAYERS]; 
  std::vector<float> m_VertexData;
  
  unsigned int m_Vbo;
  unsigned int m_Vao;
  
  void InitChunk();

  void push(float x, float y, float u, float v);
  
  void BakeVertices();
  
  void ReBakeVertices(int i, int j, int k);

  void RenderChunk(std::vector<uint8_t>& m_LayerVisibility);

  bool Collide(int i, int j, float xworld, float yworld);
  
  //void Update(GLFWwindow* window, float xworld, float yworld);

  void CleanupChunk();
};

