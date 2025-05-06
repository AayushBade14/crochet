#include "WindowManager.hpp"

namespace Crochet::Platform {
  
  Crochet::Core::Logger mLogger = Crochet::Core::Logger::getInstance();

  WindowManager& WindowManager::getInstance(){
    static WindowManager instance = WindowManager();
    return instance;
  }  
  
  void WindowManager::init(){
    mWindowWidth = Crochet::Core::Config::SCREEN_WIDTH;
    mWindowHeight = Crochet::Core::Config::SCREEN_HEIGHT;

    if(glfwInit()<0){
      mLogger.error("Failed to initialize GLFW!");    
    }
    else{
      mLogger.info("Successfully initialized GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,Crochet::Core::Config::GLFW_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,Crochet::Core::Config::GLFW_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    mLogger.info("Successfully set glfwWindowHints!");
    
    mWindow = glfwCreateWindow(mWindowWidth,
                               mWindowHeight,
                               "CrochetEngine",
                               NULL,
                               NULL);

    if(!mWindow){
      glfwTerminate();
      mLogger.error("Failed to create Crochet Window!");
    }
    else{
      mLogger.info("Successfully created Crochet Window!");
    }

    glfwMakeContextCurrent(mWindow);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      glfwTerminate();
      mLogger.error("Failed to initialize GLAD!");
    }
    else{
      mLogger.info("Successfully initialized GLAD!");
    }
  }

  void WindowManager::cleanup(){
    if(mWindow){
      glfwDestroyWindow(mWindow);
      glfwTerminate();
      mLogger.info("Successfully cleaned-up Crochet Window resources!");
    }
    else{
      mLogger.warn("Trying to clean-up uninitialized Crochet Window!");
    }
  }

  WindowManager::WindowManager(){
    init();
  }

  WindowManager::~WindowManager(){
    cleanup();
  }

  int WindowManager::getWindowWidth(){
    glfwGetWindowSize(mWindow,&mWindowWidth,&mWindowHeight);
    return mWindowWidth;
  }

  int WindowManager::getWindowHeight(){
    glfwGetWindowSize(mWindow,&mWindowWidth,&mWindowHeight);
    return mWindowHeight;
  }

  void WindowManager::setWindowRes(int width,int height){
    mWindowWidth = width;
    mWindowHeight = height;
    glfwSetWindowSize(mWindow,mWindowWidth,mWindowHeight);
  }

  GLFWwindow* WindowManager::getWindow(){
    return mWindow;
  }
}
