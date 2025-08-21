#pragma once

#include "../BackEnd/GL_BackEnd.h"

namespace OpenGLTypes {
  class GLVBO{
    private:
      GLBuffer mVbo;
      std::string mName = "VBO";

    public:
      GLVBO();
      ~GLVBO();
      
      const GLBuffer& GetBufferObject() const {return mVbo;}
      const std::string& GetName() const {return mName;}

      void Bind();
      void Unbind();
      
      
  };
}
