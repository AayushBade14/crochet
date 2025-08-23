#pragma once

#include "../BackEnd/GL_BackEnd.h"

namespace OpenGLTypes {
  class GLVBO{
    private:
      GLBuffer mVbo;
      std::string mName = "VBO";

    public:
      GLVBO(const std::string& name);
      ~GLVBO() = default;
      
      const GLBuffer& GetBufferObject() const {return mVbo;}
      const std::string& GetName() const {return mName;}
      GLenum GetTarget() {return mVbo.GetTarget();}
      const GLuint& GetId() const {return mVbo.GetId();}

      void Bind();
      void Unbind();
      
      void UpdateVbo(GLsizeiptr size, GLenum usage, const void* data);
      void UpdateVboPartial(GLintptr offset, GLsizeiptr size, const void* data);
      void UpdateVboNoFill(GLsizeiptr size, GLenum usage);
  };
}
