#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>

#include "./Vendor/stb_image/stb_image.h"

int Width = 1920;
int Height = 1013;
std::string Title = "crochet-editor";

GLFWwindow* window = nullptr;

struct Shader
{
  unsigned int m_ID;

  Shader(const std::string& vertPath, const std::string& fragPath)
  {
    std::string vCode = LoadFile(vertPath);
    std::string fCode = LoadFile(fragPath);
    unsigned int vert = CompileShader(vCode, true);
    unsigned int frag = CompileShader(fCode, false);

    CreateShaderProgram(vert, frag);
  }

  ~Shader()
  {
    glDeleteProgram(m_ID);
  }

  void Use()
  {
    glUseProgram(m_ID);
  }

  template <typename T>
  void SetValue(const std::string& name, const T& val)
  {
    const unsigned int loc = glGetUniformLocation(m_ID, name.c_str());

    if constexpr(std::is_same_v<T, int>) glUniform1i(loc, val);
    else if constexpr(std::is_same_v<T, bool>) glUniform1i(loc, (int)val);
    else if constexpr(std::is_same_v<T, float>) glUniform1f(loc, val);
    else if constexpr(std::is_same_v<T, glm::vec2>) glUniform2fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T, bool>) glUniform3fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T, bool>) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  }

private:

  std::string LoadFile(const std::string& path)
  {
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try
    {
      file.open(path);
      std::stringstream stream;
      stream << file.rdbuf();
      code = stream.str();
      file.close();
    }
    catch(const std::ifstream::failure& e)
    {
      std::cerr << e.what() << std::endl;
      exit(1);
    }

    return code;
  }

  unsigned int CompileShader(const std::string& src, bool isVert)
  {
    const char* code = src.c_str();
    int success;
    char infoLog[512];
    unsigned int shader = isVert? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr << infoLog << std::endl;
    }

    return shader;
  }

  void CreateShaderProgram(unsigned int vert, unsigned int frag)
  {
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vert);
    glAttachShader(m_ID, frag);
    glLinkProgram(m_ID);

    int success;
    char infoLog[512];

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
      std::cerr << infoLog << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
  }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main(void)
{ 
  
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Width,
                            Height,
                            Title.c_str(),
                            nullptr,
                            nullptr);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
  
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
