#include "Crochet.hpp"

namespace Crochet {
  
  RenderManager& mRenderManager = RenderManager::getInstance();
  Core::Logger& mLogger1 = Core::Logger::getInstance();

  Crochet& Crochet::getInstance(){
    static Crochet instance;
    return instance;
  }
  
  void Crochet::run(){
    mLogger1.info("Starting Crochet Engine initialisation!");
    mRenderManager.init();  
  }
}
