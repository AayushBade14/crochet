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

    ███      ▄█    ▄▄▄▄███▄▄▄▄      ▄████████    ▄████████ 
▀█████████▄ ███  ▄██▀▀▀███▀▀▀██▄   ███    ███   ███    ███ 
   ▀███▀▀██ ███▌ ███   ███   ███   ███    █▀    ███    ███ 
    ███   ▀ ███▌ ███   ███   ███  ▄███▄▄▄      ▄███▄▄▄▄██▀ 
    ███     ███▌ ███   ███   ███ ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   
    ███     ███  ███   ███   ███   ███    █▄  ▀███████████ 
    ███     ███  ███   ███   ███   ███    ███   ███    ███ 
   ▄████▀   █▀    ▀█   ███   █▀    ██████████   ███    ███ 
                                                ███    ███ 

[+] This is a timer subsystem for the crochet engine. Following are it's features:
    * Keeps track of global time.
    * keeps track of elapsed time.
    * Can be reset to 0.

[+] This timer subsystem utilizes the singleton architecture, so that the clock is globalized.
 */

#pragma once

#include "../Logger/Logger.hpp"
#include <GLFW/glfw3.h>

namespace Crochet::Core {

  class Timer{
    public:
      static Timer& getInstance();

      void start();
      
      double getElapsedTime() const;

      void reset();

    private:
      Timer();
      double m_startTime;
  };

}
