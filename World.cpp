#include "./World.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

void World::Init()
{
  /*
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
  */
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

void World::AddChunk(float x, float y)
{
  /*
  m_Chunks.emplace_back();
  Chunk& chnk = m_Chunks.back();
  chnk.m_XPos = x;
  chnk.m_YPos = y;
  chnk.InitChunk();
  chnk.BakeVertices();
  */
  
  // add a chunk only if it wasn't present already
  ChunkCoords cc{(int)x, (int)y};

  auto it = m_Chunks.find(cc);
  
  if(it == m_Chunks.end()){

    Chunk& chunk = m_Chunks[cc];
    chunk.m_ChunkCoords = cc;
    chunk.InitChunk();
    chunk.BakeVertices();
  }
}

void World::EraseChunk(float x, float y)
{
  /*
      auto it = std::find_if(m_Chunks.begin(), m_Chunks.end(), [x, y](const Chunk& c)
    {
        return c.m_XPos == x && c.m_YPos == y;
    });

    if(it != m_Chunks.end())
    {
        //it->Cleanup(); // delete VAO/VBO if you have a cleanup method
        // swap with last element then pop — avoids shifting the whole vector
        std::iter_swap(it, m_Chunks.end() - 1);
        m_Chunks.pop_back();
    }
  */
  
  auto it = m_Chunks.find({(int)x,(int)y});
  if(it != m_Chunks.end())
  {
    m_Chunks.erase(it);
  }
}

void World::Render(GLFWwindow* window, float mX, float mY, float cx, float cy, float z, float aspect, bool uifree)
{
  float chunkSize = CHUNK_SIZE * TILE_SIZE;
  float cw = 16 * TILE_SIZE * z * aspect;
  float ch = 16 * TILE_SIZE * z;

  int startX = (int)floor(cx / chunkSize);
  int endX = (int)floor((cx + cw) / chunkSize);

  int startY = (int)floor(cy / chunkSize);
  int endY = (int)floor((cy + ch) / chunkSize);

  /*
  startX = std::max(0, startX);
  startY = std::max(0, startY);

  endX = std::min(WORLD_SIZE - 1, endX);
  endY = std::min(WORLD_SIZE - 1, endY);
*/

  for(int y = startY; y <= endY; y++)
  {
    for(int x = startX; x <= endX; x++)
    {
      float m_X = x * chunkSize;
      float m_Y = y * chunkSize;
      
      auto it = m_Chunks.find({(int)m_X, (int)m_Y});
      if(it != m_Chunks.end()){
        Chunk& chunk = it->second;

        if(uifree)
          chunk.Update(window, mX, mY);
        chunk.RenderChunk();
      }
    }
  }

  /*
  // TO-DO: Implement unordered_map based sol this is just a hack
  for(auto& chunk : m_Chunks)
  {
    if(uifree)
      chunk.Update(window, mX, mY);
    chunk.RenderChunk();
  }
  */
}
