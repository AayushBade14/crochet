#include "Logger.hpp"

namespace Crochet::Core {
  
  const std::string WHITE   = "\033[0m";
  const std::string YELLOW  = "\033[38;2;255;255;0m";
  const std::string GREEN   = "\033[38;2;0;255;0m";
  const std::string RED     = "\033[38;2;255;0;0m";
 
  Logger& Logger::getInstance(){
    static Logger instance;
    return instance;
  }

  void Logger::log(const std::string& msg,LogLevel level){
    
    switch (level) {
      case LogLevel::INFO:
        std::cout << GREEN << "[INFO]: " << msg << WHITE << std::endl;
        break;
      case LogLevel::WARNING:
        std::cout << YELLOW << "[WARNING]: " << msg << WHITE << std::endl;
        break;
      case LogLevel::ERROR:
        std::cerr << RED << "[ERROR]: " << msg << WHITE << std::endl;
        std::abort();
        break;
      default:
        std::cout << GREEN << "[INFO]: " << msg << WHITE << std::endl;     
        break;
    }

  }

  void Logger::info(const std::string& msg){
    Logger::log(msg,LogLevel::INFO);
  }

  void Logger::warn(const std::string& msg){
    Logger::log(msg,LogLevel::WARNING);
  }

  void Logger::error(const std::string& msg){
    Logger::log(msg,LogLevel::ERROR);
  }

}
