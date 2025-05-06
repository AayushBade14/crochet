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
-----------------------------------------------------------------------------------------------
[+] This is a shader class, which is an abstraction to load vertex and fragment shaders written in GLSL.

[+] Optimized for use in crochet-engine.
-----------------------------------------------------------------------------------------------
 */

#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace Crochet::Graphics {
  class Shader{
    public:
      Shader(const char *vertexPath,const char *fragmentPath);
      ~Shader();

      void use();
      
      void setBool(const std::string& name,bool value) const;
      void setInt(const std::string& name,int value) const;
      void setFloat(const std::string& name,float value) const;

      void setVec2(const std::string& name,float x,float y) const;
      void setVec2(const std::string& name,glm::vec2 &vector) const;

      void setVec3(const std::string& name,float x,float y,float z) const;
      void setVec3(const std::string& name,glm::vec3 &vector) const;

      void setMat4(const std::string& name,glm::mat4 &matrix) const;
    
    private:
      unsigned int mProgramID;

      std::string loadFile(const std::string& path);
      unsigned int compileShader(unsigned int type,const std::string &source);
      void linkProgram(unsigned int vertexShader,unsigned int fragmentShader);  

  };
}
