#include "./GL_BackEnd.h"

namespace OpenGLBackEnd {
  void Init(){
    GLFWBackEnd::MakeContextCurrent();
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout<<"ERROR: GLAD::Init()!"<<std::endl;
      return;
    }
  }
}
