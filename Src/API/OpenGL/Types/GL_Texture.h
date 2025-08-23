#pragma once

#include <API/OpenGL/BackEnd/GL_BackEnd.h>
#include <vector>
#include <Vendor/Stb/stb_image.h>

namespace OpenGLTypes {
  class GLTexture{
    private:
      // texture properties
      GLuint mId;
      std::string mName;
      GLenum mType;
      std::vector<std::string> mPaths;
      
    public:
      // constructor
      GLTexture(const std::string& path); // 2d texture constructor
      GLTexture(const std::vector<std::string>& paths); // cubemap constructor
      ~GLTexture(); // destructor
      
      // getter methods
      const GLuint& GetId() const {return mId;}
      const std::string& GetName() const {return mName;}
      const GLenum& GetType() const {return mType;}
      const std::vector<std::string>& GetTexturePaths() const {return mPaths;}
      
      // methods for setting texture parameters
      void SetPWrapS(GLenum flag);
      void SetPWrapT(GLenum flag);
      void SetPWrapR(GLenum flag);

      void SetPMinFilter(Glenum flag);
      void SetPMagFilter(GLenum flag);
      
      // methods to bind and unbind a texture
      void BindTexture();
      void UnbindTexture();
      
      // assign the currently bound texture to a texture unit (MAX 15 - depends on system)
      void AssignTextureUnit(int value);
      void SetSamplerValue(GLuint shader, int value); // set the sampler value via uniforms
      
      void LoadTexture(const std::string& path, bool mipmap); // load 2d textures
      void LoadCubemapTextures(const std::vector<std::string>& paths); // load cubemap textures 6x
      
      void CreateTextureBuffer(GLenum flag, GLsizei width, GLsizei height);
  };
}
