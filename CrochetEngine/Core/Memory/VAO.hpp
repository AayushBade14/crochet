#pragma once

#include <glad/glad.h>
#include "./VBO.hpp"

namespace Crochet::Core {
  class VAO{
    public:
      VAO(VBO &vbo);
      
      void bind();
      void unbind();

      unsigned int getID();
      
      void setAttribPointer(int attribNo, int attribCount,int stride, int start);
      
      void cleanup();
    private:
      unsigned int ID;
  };
}
