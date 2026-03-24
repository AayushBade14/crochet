#include "./Chunk.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

void Chunk::InitChunk()
{
  glGenVertexArrays(1, &m_Vao);
  glGenBuffers(1, &m_Vbo);
    
  for(int i = 0; i < CHUNK_SIZE; i++)
  {
    for(int j = 0; j < CHUNK_SIZE; j++)
    {
      m_Tiles[j][i].m_ID = rand() % 10;
    }
  }
}

void Chunk::push(float x, float y, float u, float v)
{
  m_VertexData.push_back(x);
  m_VertexData.push_back(y);
  m_VertexData.push_back(u);
  m_VertexData.push_back(v);
}

void Chunk::BakeVertices()
{
  m_VertexData.clear();
  for(int i = 0; i < CHUNK_SIZE; i++)
  {
    for(int j = 0; j < CHUNK_SIZE; j++)
    {
      Tile tile = m_Tiles[j][i];
      
      float TileStartX = m_XPos + j * TILE_SIZE;
      float TileStartY = m_YPos + i * TILE_SIZE;

      float x1 = TileStartX;
      float y1 = TileStartY;

      float x2 = TileStartX + TILE_SIZE;
      float y2 = TileStartY;

      float x3 = x2;
      float y3 = TileStartY + TILE_SIZE;

      float x4 = x1;
      float y4 = y3;
      
      int tileX = tile.m_ID % 3;
      int tileY = tile.m_ID / 3;

      float u1 = (tileX * TILE_SIZE) / 96.0f;
      float v1 = (tileY * TILE_SIZE) / 96.0f;
      float u2 = ((tileX + 1) * TILE_SIZE) / 96.0f;
      float v2 = ((tileY + 1) * TILE_SIZE) / 96.0f;
      
      push(x1, y1, u1, v1);
      push(x2, y2, u2, v1);
      push(x3, y3, u2, v2);

      push(x3, y3, u2, v2);
      push(x4, y4, u1, v2);
      push(x1, y1, u1, v1);
    }
  }

  glBindVertexArray(m_Vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, m_VertexData.size() * sizeof(float), m_VertexData.data(), GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

  glBindVertexArray(0);
}

void Chunk::RenderChunk()
{
  glBindVertexArray(m_Vao);
  glDrawArrays(GL_TRIANGLES, 0, m_VertexData.size()/4);
  glBindVertexArray(0);
}

bool Chunk::Collide(int i, int j, float xMouseWorld, float yMouseWorld)
{
  float x1 = j * TILE_SIZE;
  float x2 = x1 + TILE_SIZE;

  float y1 = i * TILE_SIZE;
  float y2 = y1 + TILE_SIZE;

  return ((xMouseWorld >= x1)&&(xMouseWorld <= x2)) && ((yMouseWorld >= y1)&&(yMouseWorld <= y2))? true : false;
}

void Chunk::Update(GLFWwindow* window, float xworld, float yworld)
{
  for(int i = 0; i < CHUNK_SIZE; i++)
  {
    for(int j = 0; j < CHUNK_SIZE; j++)
    {
      if(Collide(i,j,xworld, yworld))
      {
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS)
        {
          m_Tiles[j][i].m_ID = (m_Tiles[j][i].m_ID + 1) % 10;
          BakeVertices();
        }
      }
    }
  }
}

