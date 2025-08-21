#include "./GLFW_BackEnd.h"

namespace GLFWBackEnd {
  GLFWwindow* mWindow = nullptr;

  int mWindowWidth = 1920;
  int mWindowHeight = 1013;
  const char* mWindowTitle = "crochet";

  bool Init(){
    if(glfwInit()<0){
      std::cout<<"ERROR: GLFW::Init()!"<<std::endl;
      return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWindowWidth,
                               mWindowHeight,
                               mWindowTitle,
                               NULL,
                               NULL);
    if(!mWindow){
      std::cout<<"ERROR: GLFW::Window::Create()!"<<std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(mWindow);

    std::cout<<"GLFW::Init() Successful!"<<std::endl;
    return true;
  }

  void Destroy(){
    glfwDestroyWindow(mWindow);
    glfwTerminate();

    std::cout<<"GLFW::Window::Destroy() Successful!"<<std::endl;
  }

  void* GetWindowPointer(){
    return mWindow;
  }

  int GetWindowWidth(){
    return mWindowWidth;
  }

  int GetWindowHeight(){
    return mWindowHeight;
  }
  
  bool WindowShouldClose(){
    return glfwWindowShouldClose(mWindow);
  }

}
