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

 ▄█  ███▄▄▄▄      ▄███████▄ ███    █▄      ███     
███  ███▀▀▀██▄   ███    ███ ███    ███ ▀█████████▄ 
███▌ ███   ███   ███    ███ ███    ███    ▀███▀▀██ 
███▌ ███   ███   ███    ███ ███    ███     ███   ▀ 
███▌ ███   ███ ▀█████████▀  ███    ███     ███     
███  ███   ███   ███        ███    ███     ███     
███  ███   ███   ███        ███    ███     ███     
█▀    ▀█   █▀   ▄████▀      ████████▀     ▄████▀   
                                                   
---------------------------------------------------------------------------------------------

[+] This is an input manager implementation for crochet-engine.
  - Handles key presses.
  - custom mapping for key scancodes.

[+] It utilizes singleton architecture.
----------------------------------------------------------------------------------------------
*/

#pragma once

#include "../Core/Logger/Logger.hpp"
#include "../Core/Timer/Timer.hpp"

#include <unordered_map>

#include <GLFW/glfw3.h>

namespace Crochet::Input {
  class InputManager{
    public:
      static InputManager& getInstance();

      void init(GLFWwindow *window);
      void update();

      bool isKeyPressed(int key);
      bool isKeyJustPressed(int key);
      bool isKeyJustReleased(int key);

      bool isMousePressed(int button);
      void getMousePos(double &x,double &y);

    private:
      GLFWwindow *mWindow = nullptr;
      
      std::unordered_map<int,bool> mCurrentKeys;
      std::unordered_map<int,bool> mPreviousKeys;
      
      std::unordered_map<int,bool> mCurrentMouse;
      std::unordered_map<int,bool> mPreviousMouse;

      InputManager() = default;
      InputManager(const InputManager&) = delete;
      InputManager& operator=(const InputManager&) = delete;
  };
}
