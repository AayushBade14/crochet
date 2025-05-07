#include "Crochet.hpp"

namespace Crochet {
  
  RenderManager& mRenderManager = RenderManager::getInstance();
  
  Crochet& Crochet::getInstance(){
    static Crochet instance;
    return instance;
  }
  
  void Crochet::run(){
    mRenderManager.init();  
  }
}
