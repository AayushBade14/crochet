#pragma once

#include <API/OpenGL/BackEnd/GL_BackEnd.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace OpenGLTypes {
  
  // enum class for specifying types of shader during compilation
  enum class GLShaderType{
    VERT,
    GEOM,
    FRAG
  };

  class GLShader{
    private:
      // shader properties
      GLuint mId;
      std::string mName;
      std::string mVertPath = "NA";
      std::string mGeomPath = "NA";
      std::string mFragPath = "NA";
      
      // helper methods
      std::string LoadFile(const std::string& path);
      GLuint CompileShader(const std::string& srcCode, GLShaderType type);
      void CreateShaderProgram(GLuint& vert, GLuint& frag);
      void CreateShaderProgram(GLuint& vert, GLuint& geom, GLuint& frag);

    public:
      // constructors
      GLShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
      GLShader(const std::string& name, const std::string& vertPath, const std::string& geomPath, const std::string& fragPath);
      ~GLShader(); // destructor
      
      // getter methods
      const GLuint& GetId() const {return mId;}
      const std::string& GetName() const {return mName;}
      const std::string& GetVertPath() const {return mVertPath;}
      const std::string& GetGeomPath() const {return mGeomPath;}
      const std::string& GetFragPath() const {return mFragPath;}

      void Use(); // method to set the active shader to current shader
      
      // templated method for setting uniform values
      template <typename T>
      void SetValue(const std::string& name, const T& val){
        GLuint loc = glGetUniformLocation(mId, name.c_str());
        
        if constexpr(std::is_same_v<T,int>)
          glUniform1i(loc,val);
        else if constexpr(std::is_same_v<T,bool>)
          glUniform1i(loc,(int)val);
        else if constexpr(std::is_same_v<T,float>)
          glUniform1f(loc,val);
        else if constexpr(std::is_same_v<T,glm::vec2>)
          glUniform2fv(loc,1,glm::value_ptr(val));
        else if constexpr(std::is_same_v<T,glm::vec3>)
          glUniform3fv(loc,1,glm::value_ptr(val));
        else if constexpr(std::is_same_v<T,glm::vec4>)
          glUniform3fv(loc,1,glm::value_ptr(val));
        else if constexpr(std::is_same_v<T,glm::mat4>)
          glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(val));
      }
  
  };
}
