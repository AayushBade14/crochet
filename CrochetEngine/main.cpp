#include "./Core/Crochet/Crochet.hpp"

int main(void){
  Crochet::Crochet& mCrochetEngine = Crochet::Crochet::getInstance();
  mCrochetEngine.run();
  return 0;
}
