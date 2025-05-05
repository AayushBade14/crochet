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

 ▄█        ▄██████▄     ▄██████▄     ▄██████▄     ▄████████    ▄████████ 
███       ███    ███   ███    ███   ███    ███   ███    ███   ███    ███ 
███       ███    ███   ███    █▀    ███    █▀    ███    █▀    ███    ███ 
███       ███    ███  ▄███         ▄███         ▄███▄▄▄      ▄███▄▄▄▄██▀ 
███       ███    ███ ▀▀███ ████▄  ▀▀███ ████▄  ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   
███       ███    ███   ███    ███   ███    ███   ███    █▄  ▀███████████ 
███▌    ▄ ███    ███   ███    ███   ███    ███   ███    ███   ███    ███ 
█████▄▄██  ▀██████▀    ████████▀    ████████▀    ██████████   ███    ███ 
▀                                                             ███    ███ 
------------------------------------------------------------------------------------------

This is a logging subsystem for the crochet engine. It logs all the initialization events, gives warnings and terminates the engine incase of initialization failure or any fatal error.
 */

#pragma once

#include <iostream>
#include <string>

namespace Crochet::Core {
  
  enum class LogLevel{
    INFO,
    WARNING,
    ERROR
  };

  class Logger{
    public:
      static Logger& getInstance();

      void log(const std::string& msg,LogLevel level = LogLevel::INFO);
      void info(const std::string& msg);
      void warn(const std::string& msg);
      void error(const std::string& msg);

    private:
      Logger() = default; // default constructor
      Logger(const Logger&) = delete; // disabling the copy constructor
      Logger& operator=(const Logger&) = delete; // disabling the copy assignment operator
  };
}
