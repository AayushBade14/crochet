#include <Renderer/Renderer.h>

void Render(){
  
  if(GLFWBackEnd::Init()){
    std::cout<<"GLFW::INIT SUCCESS!"<<std::endl;
  }
  OpenGLBackEnd::Init();

  
  while(!GLFWBackEnd::WindowShouldClose()){

  }
        
}
