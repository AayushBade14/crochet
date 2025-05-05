#include "Timer.hpp"

namespace Crochet::Core {
  Logger c_logger = Logger::getInstance();

  Timer& Timer::getInstance(){
    static Timer instance;
    return instance;
  }
  
  Timer::Timer() : m_startTime(0.0){}

  void Timer::start(){
    m_startTime = glfwGetTime();
    c_logger.info("Initialised Crochet-Timer!");    
  }

  double Timer::getElapsedTime() const{
    return (double)(glfwGetTime() - m_startTime);
  }

  void Timer::reset(){
    start();
    c_logger.warn("Crochet-Timer Reset!");
  }
}
