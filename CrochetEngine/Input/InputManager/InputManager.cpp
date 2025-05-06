#include "Input.hpp"

namespace Crochet::Input {
  Crochet::Core::Logger& mLogger = Crochet::Core::Logger::getInstance();

  InputManager& InputManager::getInstance(){
    static InputManager instance;
    return instance;
  }

  void InputManager::init(GLFWwindow *window){
    mWindow = window;
    mLogger.info("Successfully initialized Crochet InputManager!");
  }

  void InputManager::update(){
    mPreviousKeys = mCurrentKeys;
    mPreviousMouse = mCurrentMouse;

    for(int key = GLFW_KEY_SPACE;key<=GLFW_KEY_LAST;key++){
      mCurrentKeys[key] = glfwGetKey(mWindow,key) == GLFW_PRESS;
    }

    for(int button = GLFW_MOUSE_BUTTON_1;button<=GLFW_MOUSE_BUTTON_LAST;button++){
      mCurrentMouse[button] = glfwGetMouseButton(mWindow,button) == GLFW_PRESS;
    }
  }

  void InputManager::isKeyPressed(int key){
    return mCurrentKeys[key];
  }

  void InputManager::isKeyJustPressed(int key){
    return mCurrentKeys[key] && !mPreviousKeys[key];
  }

  void InputManager::isKeyJustReleased(int key){
    return mPreviousKeys[key] && !mCurrentKeys[key];
  }

  void InputManager::isMousePressed(int button){
    return mCurrentMouse[button];
  }

  void InputManager::getMousePos(double &x,double &y){
    glfwGetCursorPos(mWindow,&x,&y);
  }
}
