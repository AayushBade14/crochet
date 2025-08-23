#include <API/OpenGL/Types/GL_VAO.h>

namespace OpenGLTypes {
  GLVAO::GLVAO(const std::string& name): mName(name) {
    glGenVertexArrays(1,&mId);
  }
  
  GLVAO::~GLVAO(){
    glDeleteVertexArrays(1,&mId); 
  }

  void GLVAO::Bind(){
    glBindVertexArray(mId);
  }
  
  void GLVAO::Unbind(){
    glBindVertexArray(0);
  }

  void GLVAO::SetAttribPointer(GLuint index, GLint size, GLsizei stride, GLsizei offset){
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(intptr_t)offset);
  }

  void GLVAO::SetAttribIPointer(GLuint index, GLint size, GLsizei stride, GLsizei offset){
    glEnableVertexAttribArray(index);
    glVertexAttribIPointer(index, size, GL_INT, stride, (void*)(intptr_t)offset);
  }
  
  void GLVAO::SetAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset){
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, (void*)(intptr_t)offset);
  }

  void GLVAO::Draw(GLenum mode, GLint first, GLsizei count){
    Bind();
    glDrawArrays(mode, first, count);
    Unbind();
  }
  
  void GLVAO::DrawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices){
    Bind();
    glDrawElements(mode, count, type, indices);
    Unbind();
  }

  void GLVAO::InstancedDraw(GLenum mode, GLint first, GLsizei count, GLsizei instances){
    Bind();
    glDrawArraysInstanced(mode, first, count, instances);
    Unbind();
  }
  
  void GLVAO::InstancedDrawIndexed(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instances){
    Bind();
    glDrawElementsInstanced(mode, count, type, indices, instances);
  }

  void GLVAO::SetAttribDivisor(GLuint index, GLuint value){
    glAttribDivisor(index, value);
  }
}
