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
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
  }

  void GLVAO::SetAttribIPointer(GLuint index, GLint size, GLsizei stride, GLsizei offset){
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_INT, stride, (void*)offset);
  }
  
  void GLVAO::SetAttribIPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset){
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
  }
}
