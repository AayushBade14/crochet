#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

enum class ShaderType{
  VERT,
  GEOM,
  FRAG
};

class Shader{
  private:
    std::string mName;
    unsigned int mId;
    
    std::string LoadFile(const std::string& path);
    unsigned int CompileShader(const std::string& srcCode, ShaderType type);
    void CreateShaderProgram(const unsigned int& vert, const unsigned int& frag);
    void CreateShaderProgram(const unsigned int& vert, const unsigned int& geom, const unsigned int& frag);

  public:
    Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
    Shader(const std::string& name, const std::string& vertPath, const std::string& geomPath, const std::string& fragPath);

    ~Shader();

    void Use();

    template <typename T>
    void SetValue(const std::string& name, const T& val){
      unsigned int loc = glGetUniformLocation(mId, name.c_str());

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
      else if constexpr(std::is_same_v<T,glm::mat4>)
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(val));
    }
};

class VBO{
  private:
    std::string mName;  
    GLenum mType;
    unsigned int mId;
     
  public:
    VBO(const std::string& name);
    ~VBO();

    void Bind();
    void Unbind();

    void AllocateAndFillMemory(GLenum usage, size_t size, const void* data);
    void AllocateMemory(GLenum usage, size_t size);
    void FillPartialMemory(size_t size, size_t offset, const void* data);
};
