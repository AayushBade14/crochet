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

   ▄████████    ▄█    █▄       ▄████████ ████████▄     ▄████████    ▄████████ 
  ███    ███   ███    ███     ███    ███ ███   ▀███   ███    ███   ███    ███ 
  ███    █▀    ███    ███     ███    ███ ███    ███   ███    █▀    ███    ███ 
  ███         ▄███▄▄▄▄███▄▄   ███    ███ ███    ███  ▄███▄▄▄      ▄███▄▄▄▄██▀ 
▀███████████ ▀▀███▀▀▀▀███▀  ▀███████████ ███    ███ ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   
         ███   ███    ███     ███    ███ ███    ███   ███    █▄  ▀███████████ 
   ▄█    ███   ███    ███     ███    ███ ███   ▄███   ███    ███   ███    ███ 
 ▄████████▀    ███    █▀      ███    █▀  ████████▀    ██████████   ███    ███ 
                                                                   ███    ███ 

   ▄▄▄▄███▄▄▄▄      ▄████████ ███▄▄▄▄      ▄████████    ▄██████▄     ▄████████    ▄████████ 
 ▄██▀▀▀███▀▀▀██▄   ███    ███ ███▀▀▀██▄   ███    ███   ███    ███   ███    ███   ███    ███ 
 ███   ███   ███   ███    ███ ███   ███   ███    ███   ███    █▀    ███    █▀    ███    ███ 
 ███   ███   ███   ███    ███ ███   ███   ███    ███  ▄███         ▄███▄▄▄      ▄███▄▄▄▄██▀ 
 ███   ███   ███ ▀███████████ ███   ███ ▀███████████ ▀▀███ ████▄  ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   
 ███   ███   ███   ███    ███ ███   ███   ███    ███   ███    ███   ███    █▄  ▀███████████ 
 ███   ███   ███   ███    ███ ███   ███   ███    ███   ███    ███   ███    ███   ███    ███ 
  ▀█   ███   █▀    ███    █▀   ▀█   █▀    ███    █▀    ████████▀    ██████████   ███    ███ 
                                                                                 ███    ███ 
-------------------------------------------------------------------------------------------

[+] This is a shader manager class for crochet engine.

[+] This manages shader objects in an optimized manner.
 ------------------------------------------------------------------------------------------
 */

#pragma once

#include "Shader.hpp"

#include <unordered_map>

namespace Crochet::Graphics {
  class ShaderManager{
    public:
      static ShaderManager& getInstance();
      
      void loadShader(const std::string& name,const std::string& vertexPath,const std::string& fragmentPath);

      Shader& getShader(const std::string& name);

    private:
      ShaderManager() = default;
      std::unordered_map<string,Shader> mShaders;
      ShaderManager(const ShaderManager&) = delete;
      ShaderManager& operator=(const ShaderManager&) = delete;
  };
}
