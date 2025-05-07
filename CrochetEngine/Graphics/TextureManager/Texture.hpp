/*
  
 ▄████████    ▄████████  ▄██████▄   ▄████████    ▄█    █▄       ▄████████     ███     
███    ███   ███    ███ ███    ███ ███    ███   ███    ███     ███    ███ ▀█████████▄ 
███    █▀    ███    ███ ███    ███ ███    █▀    ███    ███     ███    █▀     ▀███▀▀██ 
███         ▄███▄▄▄▄██▀ ███    ███ ███         ▄███▄▄▄▄███▄▄  ▄███▄▄▄         ███   ▀ 
███        ▀▀███▀▀▀▀▀   ███    ███ ███        ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀         ███     
███    █▄  ▀███████████ ███    ███ ███    █▄    ███    ███     ███    █▄      ███     
███    ███   ███    ███ ███    ███ ███    ███   ███    ███     ███    ███     ███     
████████▀    ███    ███  ▀██████▀  ████████▀    ███    █▀      ██████████    ▄████▀   
             ███    ███                                                               

    ███        ▄████████ ▀████    ▐████▀     ███     ███    █▄     ▄████████    ▄████████ 
▀█████████▄   ███    ███   ███▌   ████▀  ▀█████████▄ ███    ███   ███    ███   ███    ███ 
   ▀███▀▀██   ███    █▀     ███  ▐███       ▀███▀▀██ ███    ███   ███    ███   ███    █▀  
    ███   ▀  ▄███▄▄▄        ▀███▄███▀        ███   ▀ ███    ███  ▄███▄▄▄▄██▀  ▄███▄▄▄     
    ███     ▀▀███▀▀▀        ████▀██▄         ███     ███    ███ ▀▀███▀▀▀▀▀   ▀▀███▀▀▀     
    ███       ███    █▄    ▐███  ▀███        ███     ███    ███ ▀███████████   ███    █▄  
    ███       ███    ███  ▄███     ███▄      ███     ███    ███   ███    ███   ███    ███ 
   ▄████▀     ██████████ ████       ███▄    ▄████▀   ████████▀    ███    ███   ██████████ 
                                                                  ███    ███              
---------------------------------------------------------------------------------------------

[+] This is a texture class customised for the crochet engine.

[+] This class will be responsible to hold texture images in the necessary format for the engine to render.
----------------------------------------------------------------------------------------------
 */

#pragma once

#include <glad/glad.h>

#include <stbi_image.h>

#include "../ShaderManager/Shader.hpp"

namespace Crochet::Graphics {
  class Texture{
    public:
      Texture(const std::string& path,const std::string& samplerName,int val,Shader& shader);
      ~Texture();
      
      void bind();

    private:
      unsigned int mTextureID;
      int val = 0;
      const std::string path;
      const std::string samplerName;
      Shader shader;

      void loadTexture(const std::string& path,bool isPNG);
      void bindSampler(const std::string& samplerName,int val,Shader& shader);
  };
}
