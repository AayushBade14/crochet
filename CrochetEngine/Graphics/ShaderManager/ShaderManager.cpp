#include "ShaderManager.hpp"

namespace Crochet::Core::Graphics {
  
  Shader& Shader::getInstance(){
    static Shader instance;
    return instance;
  }
  
  void Shader::loadShader(const std::string& name,const std::string& vertexPath,const std::string& fragmentPath){
    Shader shader = Shader(vertexPath,fragmentPath);
    mShaders[name] = shader; 
  }

  Shader& Shader::getShader(const std::string& name){
    return mShaders[name];
  }
}
