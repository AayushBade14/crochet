#include "./VBO.hpp"

namespace Crochet::Core {
  VBO::VBO(std::vector<float> &vertices){
    glGenBuffers(1,&ID);
    glBindBuffer(GL_ARRAY_BUFFER,ID);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float),vertices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }

  unsigned int VBO::getID(){
    return ID;
  }

  void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER,ID);
  }

  void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }

  void VBO::cleanup(){
    glDestroyBuffers(1,&ID);
  }
}
