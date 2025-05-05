#include "Logger.hpp"

namespace Crochet::Core {
  
  const std::string WHITE   = "\033[0m";
  const std::string YELLOW  = "\033[33m";
  const std::string GREEN   = "\033[32m";
  const std::string RED     = "\033[31m";
  
  Logger& Logger::getInstance(){
    static Logger instance;
    return instance;
  }

  void Logger::log(const std::string& msg,LogLevel level = INFO){
    
    switch (level) {
      case LogLevel::INFO:
        std::cout << GREEN << "[INFO]: " << msg << WHITE << std::endl;
        break;
      case LogLevel::WARN:
        std::cout << YELLOW << "[WARNING]: " << msg << WHITE << std::endl;
        break;
      case LogLevel::ERROR:
        std::cerr << RED << "[ERROR]: " << msg << WHITE << std::endl;
        std::abort(1);
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
    Logger::log(msg,LogLevel::WARN);
  }

  void Logger::error(const std::string& msg){
    Logger::log(msg,LogLevel::ERROR);
  }

}
