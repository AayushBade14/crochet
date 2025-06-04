#pragma once

#include <glad/glad.h>
#include <vector>

namespace Crochet::Core{
  class VBO{
    public:
      VBO(std::vector<float> &vertices);
      
      unsigned int getID();

      void bind();
      void unbind();
      
      void cleanup();

    private:
      unsigned int ID;      
  }; 
}
