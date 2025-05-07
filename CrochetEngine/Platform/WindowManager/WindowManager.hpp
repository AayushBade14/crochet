/*
 
 ▄████████    ▄████████  ▄██████▄   ▄████████    ▄█    █▄       ▄████████     ███     
███    ███   ███    ███ ███    ███ ███    ███   ███    ███     ███    ███ ▀█████████▄ 
███    █▀    ███    ███ ███    ███ ███    █▀    ███    ███     ███    █▀     ▀███▀▀██ 
███         ▄███▄▄▄▄██▀ ███    ███ ███         ▄███▄▄▄▄███▄▄  ▄███▄▄▄         ███   ▀ 
███        ▀▀███▀▀▀▀▀   ███    ███ ███        ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀         ███     
███    █▄  ▀███████████ ███    ███ ███    █▄    ███    ███     ███    █▄      ███     
███    ███   ███    ███ ███    ███ ███    ███   ███    ███     ███    ███     ███     
████████▀    ███    ███  ▀██████▀  ████████▀    ███    █▀      ██████████    ▄████▀   
             ███    ███                                                               

 ▄█     █▄    ▄▄▄▄███▄▄▄▄   
███     ███ ▄██▀▀▀███▀▀▀██▄ 
███     ███ ███   ███   ███ 
███     ███ ███   ███   ███ 
███     ███ ███   ███   ███ 
███     ███ ███   ███   ███ 
███ ▄█▄ ███ ███   ███   ███ 
 ▀███▀███▀   ▀█   ███   █▀  
                            
-------------------------------------------------------------------------------------------

[+] This is a window manager sub-system for the crochet game engine.

 */

#pragma once

#include "../Core/Logger/Logger.hpp"
#include "../Core/Config/Config.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/Timer/Timer.hpp"

namespace Crochet::Platform {
  class WindowManager{
    public:  
      static WindowManager& getInstance();
      
      void init();
      void cleanup();

      GLFWwindow *getWindow();
      
      int getWindowWidth();
      int getWindowHeight();
      
      void setWindowRes(int width,int height);

      bool shouldClose();
      
      void pollEvents();
      void swapBuffers();

    private:
      WindowManager() = default;
      WindowManager(const WindowManager&) = delete;
      WindowManager& operator=(const WindowManager&) = delete;

      GLFWwindow *mWindow = nullptr;
      int mWindowWidth = 0;
      int mWindowHeight = 0;
      
      static void framebuffer_size_callback(GLFWwindow *window,int width,int height);  
    };
}

