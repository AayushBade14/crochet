#include "./GL_BackEnd.h"

namespace OpenGLBackEnd {
  void Init(){
    GLFWBackEnd::MakeContextCurrent();
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout<<"ERROR: GLAD::Init()!"<<std::endl;
      return;
    }
  }

  void* GetMemoryPointer(GLenum target, GLenum access){
    return glMapBuffer(target, access);
  }

  void  UnmapMemoryPointer(GLenum target){
    glUnmapBuffer(target);
  }

  void FillMemoryManual(void* ptr, const void* data, int offset){
    memcpy(ptr+offset, data, sizeof(data));
  }
  
  void BindBuffer(GLenum target,GLuint id){
    glBindBuffer(target, id);
  }

  void UnbindBuffer(GLenum target){
    glBindBuffer(target, 0);
  }
  
  void CopyBufferData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size){
    glCopyBufferSubData(readtarget, writetarget, readoffset, writeoffset, size);
  }

  GLBuffer::GLBuffer(GLenum target){
    switch (target) {
      case GL_ARRAY_BUFFER:
        glGenBuffers(1,&mId);
        break;
      case GL_ELEMENT_ARRAY_BUFFER:
        glGenBuffers(1,&mId);
        break;
      case GL_FRAMEBUFFER:
        glGenFramebuffers(1,&mId);
        break;
      case GL_RENDERBUFFER:
        glGenRenderbuffers(1,&mId);
        break;
      case GL_UNIFORM_BUFFER:
        glGenBuffers(1,&mId);
        break;
      default:
        std::cout<<"ERROR: "<<target<<" is not a known target-type!"<<std::endl;
        break;
    }

    mTarget = target;
  }
  
  GLBuffer::~GLBuffer(){
    switch (mTarget) {
      case GL_ARRAY_BUFFER:
        glDeleteBuffers(1,&mId);
        break;
      case GL_ELEMENT_ARRAY_BUFFER:
        glDeleteBuffers(1,&mId);
        break;
      case GL_FRAMEBUFFER:
        glDeleteFramebuffers(1,&mId);
        break;
      case GL_RENDERBUFFER:
        glDeleteRenderbuffers(1,&mId);
        break;
      case GL_UNIFORM_BUFFER:
        glDeleteBuffers(1,&mId);
        break;
      default:
        std::cout<<"ERROR: "<<target<<" is not a known target-type!"<<std::endl;
        break;
    }
  }
  
  void GLBuffer::BindBuffer(){
    switch (mTarget) {
      case GL_ARRAY_BUFFER:
        glBindBuffer(mTarget, mId);
        break;
      case GL_ELEMENT_ARRAY_BUFFER:
        glBindBuffer(mTarget, mId);
        break;
      case GL_FRAMEBUFFER:
        glBindFramebuffer(mTarget, mId);
        break;
      case GL_RENDERBUFFER:
        glBindRenderbuffer(mTarget, mId);
        break;
      case GL_UNIFORM_BUFFER:
        glBindBuffer(mTarget, mId);
        break;
      default:
        std::cout<<"ERROR: "<<target<<" is not a known target-type!"<<std::endl;
        break;
    }
  }

  void GLBuffer::UnbindBuffer(){
    switch (mTarget) {
      case GL_ARRAY_BUFFER:
        glBindBuffer(mTarget, 0);
        break;
      case GL_ELEMENT_ARRAY_BUFFER:
        glBindBuffer(mTarget, 0);
        break;
      case GL_FRAMEBUFFER:
        glBindFramebuffer(mTarget, 0);
        break;
      case GL_RENDERBUFFER:
        glBindRenderbuffer(mTarget, 0);
        break;
      case GL_UNIFORM_BUFFER:
        glBindBuffer(mTarget, 0);
        break;
      default:
        std::cout<<"ERROR: "<<target<<" is not a known target-type!"<<std::endl;
        break;
    }
  }

  void GLBuffer::AllocateMemory(GLsizeiptr size, GLenum usage){
    glBufferData(mTarget, size, NULL, usage);
  }
  
  void GLBuffer::FillPartialMemory(GLintptr offset, GLsizeiptr size, const void* data){
    glBufferSubData(mTarget, offset, size, data);
  }
  
  void GLBuffer::AllocateAndFillMemory(GLsizeiptr size, GLenum usage, const void* data){
    glBufferData(mTarget, size, data, usage);
  }

  void GLBuffer::InitRBOStorate(GLenum format, GLsizei width, GLsizei height){
    glRenderbufferStorage(mTarget, format, width, height);
  }
  
  void GLBuffer::FBOAttachTexture(GLenum attachment, GLenum textarget, GLuint texture, GLint level){
    glFramebufferTexture2D(mTarget, attachment, textarget, texture, level); 
  }

  void GLBuffer::FBOAttachRBO(GLenum attachment, GLuint rbo){
    glFramebufferRenderbuffer(mTarget, attachment, GL_RENDERBUFFER, rbo);
  }

  GLuint GLBuffer::GetUniformBlockIndex(GLuint shader, GLchar* blockname){
    return glGetUniformBlockIndex(shader, blockname);
  }
  
  void GLBuffer::SetUniformBlockBindingPoint(GLuint shader, GLuint ubindex, GLuint bindingpoint){
    glUniformBlockBinding(shader, ubindex, bindingpoint);
  }
  
  void GLBuffer::SetUBOBindingPoint(GLuint bindingpoint){
    glBufferBase(mTarget, bindingpoint, mId);
  }
  
  void GLBuffer::SetPartialUBOBindingPoint(GLuint bindingpoint, GLintptr offset, GLsizeiptr size){
    glBufferRange(mTarget, bindingpoint, mId, offset, size);
  }

}
