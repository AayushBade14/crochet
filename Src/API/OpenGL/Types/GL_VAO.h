#pragma once

#include <API/OpenGL/BackEnd/GL_BackEnd.h>

namespace OpenGLTypes {
  class GLVAO{
    private:
      GLuint mId;
      std::string mName;

    public:
      GLVAO(const std::string& name);
      ~GLVAO();
      
      const GLuint& GetId() const {return mId;}
      const std::string& GetName() const {return mName;}

      void Bind();
      void Unbind();
  
      void SetAttribPointer(GLuint index, GLint size, GLsizei stride, GLsizei offset);
      void SetAttribIPointer(GLuint index, GLint size, GLsizei stride, GLsizei offset);
      void SetAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset);
      
      void SetAttribDivisor(GLuint index, GLuint value);
  
      void Draw(GLenum mode, GLint first, GLsizei count);
      void DrawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices);
      
      void InstancedDraw(GLenum mode, GLint first, GLsizei count, GLsizei instances);
      void InstancedDrawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instances);
  }; 
}
