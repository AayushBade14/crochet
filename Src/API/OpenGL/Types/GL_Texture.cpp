#include <API/OpenGL/Types/GL_Texture.h>

namespace OpenGLTypes {
  GLTexture::GLTexture(const std::string& path){
    glGenTextures(1, &mId);
    mPaths.push_back(path);
    mType = GL_TEXTURE_2D;
  }
  
  GLTexture::GLTexture(const std::vector<std::string>& paths): mPaths(paths){
    glGenTextures(1, &mId);
    mType = GL_TEXTURE_CUBE_MAP;
  }

  GLTexture::~GLTexture(){
    glDeleteTextures(1, &mId);
    std::cout<<"Texture::Cleanup("<<mName<<") Successful!"<<std::endl;
  }
  
  void GLTexture::LoadTexture(const std::string& path, bool mipmap){
    stbi_set_flip_vertically_on_load(true);

    int width;
    int height;
    int nrChannels;
    GLenum flag;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    switch (nrChannels) {
      case 1:
        flag = GL_RED;
        break;
      case 3:
        flag = GL_RGB;
        break;
      case 4:
        flag = GL_RGBA;
        break;
      default:
        std::cout<<"ERROR: Unsupported texture format!"<<std::endl;
        break;
    }

    if(data){
      glTexImage2D(mType, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
      if(mipmap)
        glGenerateMipmap(mType);
    }
    else{
      std::cout<<"ERROR: Load::Texture("<<mName<<") "<<"Failed!"<<std::endl;
    }
    stbi_image_free(data);

    std::cout<<"Load::Texture("<<mName<<") Successful!"<<std::endl;
  }
  
  void GLTexture::LoadCubemapTextures(const std::vector<std::string>& paths){
    stbi_set_flip_vertically_on_load(false);

    int width;
    int height;
    int nrChannels;

    for(unsigned int i = 0; i < paths.size(); i++){
      unsigned char* data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
      if(data){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      }
      else{
        std::cout<<"ERROR: Load::Texture("<<mName<<") - "<<paths[i]<<std::endl;
      }
      stbi_image_free(data);
    }

    std::cout<<"Successfully Loaded Cubemap Textures! -> "<<mName<<std::endl;
  }

  void GLTexture::SetPWrapS(GLenum flag){
    glTexParameteri(mType, GL_TEXTURE_WRAP_S, flag);
  }
  
  void GLTexture::SetPWrapT(GLenum flag){
    glTexParameteri(mType, GL_TEXTURE_WRAP_T, flag);
  }

  void GLTexture::SetPWrapR(GLenum flag){
    glTexParameteri(mType, GL_TEXTURE_WRAP_R, flag);
  }

  void GLTexture::SetPMinFilter(GLenum flag){
    glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, flag);
  }

  void GLTexture::SetPMagFilter(GLenum flag){
    glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, flag);
  }
  
  void GLTexture::BindTexture(){
    glBindTexture(mType, mId);
    std::cout<<"Bound Texture: "<<mName<<" Type: "<<mType<<std::endl;
  }

  void GLTexture::UnbindTexture(){
    glBindTexture(mType, 0);
    std::cout<<"Unbound Texture: "<<mName<<" Type: "<<mType<<std::endl;
  }

  void GLTexture::AssignTextureUnit(int value){
    glTextureActive(GL_TEXTURE0 + value);
    std::cout<<"Assigned Texture-Unit: "<<value<<" To: "<<mName<<std::endl;
  }
  
  void GLTexture::SetSamplerValue(GLuint shader, int value){
    GLuint loc = glGetUniformLocation(shader, mName.c_str());
    glUniform1i(loc, value);
    std::cout<<"Successfully set sampler value: "<<value<<" To: "<<mName<<std::endl;
  }
  
  void GLTexture::CreateTextureBuffer(GLenum flag, GLsizei width, GLsizei height){
    glTexImage2D(GL_TEXTURE_2D, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, NULL);
    std::cout<<"Successfully created texture buffer for attachment: "<<mName<<std::endl;
  }

}
