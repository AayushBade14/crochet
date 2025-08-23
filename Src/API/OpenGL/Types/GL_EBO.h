#pragma once

#include <API/OpenGL/BackEnd/GL_BackEnd.h>

namespace OpenGLTypes {
  class GLEBO{
    private:
      Buffer mEbo;
      std::string mName = "EBO";

    public:
      GLEBO(const std::string& name);
      ~GLEBO() = default;
      
      const Buffer& GetBuffer() const {return mEbo;}
      const GLuint& GetId() const {return mEbo.GetId();}
      const std::string& GetName() const {return mName;}
      GLenum GetTarget() {return mEbo.GetTarget();}
  
      void Bind();
      void Unbind();

      void UpdateEbo(GLsizeiptr size, GLenum usage, const void* data);
  };
}
