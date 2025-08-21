#pragma once

#include <glad/glad.h>
#include <iostream>
#include "../../../BackEnd/GLFW_BackEnd.h"

namespace OpenGLBackEnd {
  void Init();
  
  void* GetMemoryPointer(GLenum target, GLenum access);
  void UnmapMemoryPointer(GLenum target);
  void FillMemoryManual(void* ptr, const void* data, int offset);

  void BindBuffer(GLenum target, GLuint id);
  void UnbindBuffer(GLenum target);
  
  void CopyBufferData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeptr, GLsizeiptr size);

  class Buffer{
    private:
      GLuint mId;
      GLenum mTarget;

    public:
      Buffer(GLenum target);
      ~Buffer();
      
      const GLuint& GetId() const {return mId;}
      GLenum GetTarget() {return mTarget;}

      void BindBuffer();
      void UnbindBuffer();
      
      // VBO specific
      void AllocateMemory(GLsizeiptr size, GLenum usage);
      void FillPartialMemory(GLintptr offset, GLsizeiptr size, const void* data);
      void AllocateAndFillMemory(GLsizeiptr size, GLenum usage, const void* data);
      
      // RBO specific
      void InitRBOStorage(GLenum format, GLsizei width, GLsizei height);
      
      // FBO specific
      void FBOAttachTexture(GLenum attachment, GLenum textarget, GLuint texture,  GLint level);
      void FBOAttachRBO(GLenum attachment, GLuint rbo);
      
      // UBO specific
      GLuint GetUniformBlockIndex(GLuint shader, GLchar* blockname);
      void SetUniformBlockBindingPoint(GLuint shader, GLuint ubindex, GLuint bindingpoint);
      void SetUBOBindingPoint(GLuint bindingpoint);
      void SetPartialUBOBindingPoint(GLuint bindingpoint, GLintptr offset, GLsizeiptr size);
  };
}
