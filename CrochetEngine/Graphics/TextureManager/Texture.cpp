#include "Texture.hpp"

namespace Crochet::Graphics {
  
  void Texture::loadTexture(const std::string& path,bool isPNG){
    
    glGenTextures(1,&mTextureID);
    glBindTexture(GL_TEXTURE_2D,mTextureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_FILTER,GL_LINEAR);
    
    int width,height,nrChannels;
    unsigned char* data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);
    if(data){
      switch (isPNG) {
        case 1:
          glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
          break;
        case 0:
          glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
          break;
        default:
          glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
          break;
      }
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
      // error
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D,0);
  } 
  
  void Texture::bindSampler(const std::string& samplerName,int val,Shader& shader){
    shader.setInt(samplerName,val);
  } 

  Texture::Texture(const std::string& path,const std::string& samplerName,int val,Shader& shader){
    this->val = val;
    this->samplerName = samplerName;
    this->path = path;
    this->shader = shader;

    loadTexture(path);
    bindSampler(samplerName,val,shader);
  }

  Texture::~Texture(){
    glDeleteTextures(1,&mTextureID);
  }

  void Texture::bind(){
    glActiveTexture(GL_TEXTURE0+(this->val));
    glBindTexture(GL_TEXTURE_2D,mTextureID);
  }

  void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D,0); 
  }
}
