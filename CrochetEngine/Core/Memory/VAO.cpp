#include "./VAO.hpp"

namespace Crochet::Core {
  VAO::VAO(VBO &vbo){
    glGenVertexArrays(1,&ID);
    glBindVertexArray(ID);
    vbo.bind();

    glBindVertexArray(0);
  }

  void VAO::bind(){
    glBindVertexArray(ID);
  }

  void VAO::unbind(){
    glBindVertexArray(0);
  }

  unsigned int VAO::getID(){
    return ID;
  }

  void VAO::setAttribPointer(int attribNo, int attribCount, int stride, int start){
    this.bind();
    glSetVertexAttribPointer(attribNo,attribCount,GL_FLOAT,GL_FALSE,stride*sizeof(float),(void*)(start*sizeof(float)));
    glEnableVertexAttribArray(attribNo);

    this.unbind();
  }
  
  void VAO::cleanup(){
    glDestroyVertexArrays(1,&ID);
  }
}
