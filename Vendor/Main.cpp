#include <Include/OGL.h>
#include <GLFW/glfw3.h>

#define WIDTH 1920
#define HEIGHT 1013
#define TITLE "Crochet-Engine"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0,0,width,height);
}

int main(void){
  if(glfwInit()<0){
    std::cerr<<"ERROR: GLFW::INIT()!"<<std::endl;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,TITLE,NULL,NULL);

  if(!window){
    std::cerr<<"ERROR: Window::Create()!"<<std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    glfwTerminate();
    glfwDestroyWindow(window);
    std::cerr<<"ERROR: GLAD::INIT()!"<<std::endl;
  }

  
}
