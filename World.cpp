#include "./World.h"
#include <GLFW/glfw3.h>

void World::Init()
{
  for(int i = 0; i < WORLD_SIZE; i++)
  {
    for(int j = 0; j < WORLD_SIZE; j++)
    {
      float chunkX = j * CHUNK_SIZE * TILE_SIZE;
      float chunkY = i * CHUNK_SIZE * TILE_SIZE;
      
      m_Chunks.emplace_back();
      Chunk& chnk = m_Chunks.back();

      chnk.m_XPos = chunkX;
      chnk.m_YPos = chunkY;

      chnk.InitChunk();
      chnk.BakeVertices();
    }
  }
}
/*
bool World::IsChunkVisible(Chunk& chunk, float cx, float cy, float z, float aspect)
{
  float cw = 16 * TILE_SIZE * aspect * z;
  float ch = 16 * TILE_SIZE * z;

  float c_left = cx;
  float c_right = c_left + cw;

  float c_bottom = cy;
  float c_top = c_bottom + ch;

  float chunkSize = CHUNK_SIZE * TILE_SIZE;

  float chunk_left = chunk.m_XPos;
  float chunk_right = chunk_left + chunkSize;

  float chunk_bottom = chunk.m_YPos;
  float chunk_top = chunk_bottom + chunkSize;

  return ((chunk_right >= c_left) && (chunk_left <= c_right) && (chunk_top >= c_bottom) && (chunk_bottom <= c_top));
}
*/
void World::Render(GLFWwindow* window, float mX, float mY, float cx, float cy, float z, float aspect)
{
  float chunkSize = CHUNK_SIZE * TILE_SIZE;
  float cw = 16 * TILE_SIZE * z * aspect;
  float ch = 16 * TILE_SIZE * z;

  int startX = (int)(cx / chunkSize);
  int endX = (int)((cx + cw) / chunkSize);

  int startY = (int)(cy / chunkSize);
  int endY = (int)((cy + ch) / chunkSize);

  startX = std::max(0, startX);
  startY = std::max(0, startY);

  endX = std::min(WORLD_SIZE - 1, endX);
  endY = std::min(WORLD_SIZE - 1, endY);

  for(int y = startY; y <= endY; y++)
  {
    for(int x = startX; x <= endX; x++)
    {
      int index = y * WORLD_SIZE + x;
      m_Chunks[index].Update(window, mX, mY);
      m_Chunks[index].RenderChunk();
    }
  }
}
