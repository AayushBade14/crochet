#include "./VBO.hpp"

namespace Crochet::Core {
  VBO::VBO(std::vector<float> &vertices){
    glGenBuffers(1,&ID);
    glBindBuffer(GL_ARRAY_BUFFER,ID);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float),vertices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }

  unsigned int getID(){
    return ID;
  }

  void bind(){
    glBindBuffer(GL_ARRAY_BUFFER,ID);
  }

  void unbind(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }
}
