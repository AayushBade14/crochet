#include "TextureManager.hpp"

namespace Crochet::Graphics {
  TextureManager& TextureManager::getInstance(){
    static TextureManager instance;
    return instance;
  }

  void TextureManager::addTexture(const std::string& name,const std::string& path,const std::string& samplerName,int val,Shader& shader,bool isPNG){
    Texture texture = Texture(path,samplerName,val,shader,isPNG);
    mTextures[name] = texture;
  }
  
  Texture& getTexture(const std::string& name){
    return mTextures[name];
  }

  
}
