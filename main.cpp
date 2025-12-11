#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Shader{
  unsigned int mId;

  Shader(const std::string& vertPath, const std::string& fragPath){
    std::string vCode = LoadFile(vertPath);
    std::string fCode = LoadFile(fragPath);
    unsigned int vert = CompileShader(vCode, true);
    unsigned int frag = CompileShader(fCode, false);
    CreateShaderProgram(vert, frag);
  }

  ~Shader() {glDeleteProgram(mId);}

  void Use() {glUseProgram(mId);}

  template <typename T>
  void SetValue(const std::string& name, const T& val){
    const unsigned int loc = glGetUniformLocation(mId, name.c_str());
  
    if constexpr(std::is_same_v<T,int>) glUniform1i(loc, val);
    else if constexpr(std::is_same_v<T,bool>) glUniform1i(loc, (int)val);
    else if constexpr(std::is_same_v<T,float>) glUniform1f(loc, val);
    else if constexpr(std::is_same_v<T,glm::vec2>) glUniform2fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T,glm::vec3>) glUniform3fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T,glm::mat4>) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  }

private:
  std::string LoadFile(const std::string& path){
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try{
      file.open(path);
      std::stringstream stream;
      stream << file.rdbuf();
      file.close();
      code = stream.str();
    }
    catch(const std::ifstream::failure& e){
      exit(1);
    }

    return code;
  }

  unsigned int CompileShader(const std::string& srcCode, bool isVert){
    const char* code = srcCode.c_str();
    int success;
    char infoLog[512];
    unsigned int shader = isVert? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr<<infoLog<<std::endl;
      exit(1);
    }

    return shader;
  }

  void CreateShaderProgram(unsigned int& vert, unsigned int& frag){
    mId = glCreateProgram();
    glAttachShader(mId, vert);
    glAttachShader(mId, frag);
    glLinkProgram(mId);

    int success;
    char infoLog[512];

    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(mId, 512, nullptr, infoLog);
      std::cerr<<infoLog<<std::endl;
      exit(1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
  }
};

int WIDTH = 1920;
int HEIGHT = 1013;
const char* TITLE = "crochet";

GLFWwindow* window = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

int main(void){
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH,
                            HEIGHT,
                            TITLE,
                            nullptr,
                            nullptr);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  
  unsigned int vbo;
  unsigned int vao;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

  glBindVertexArray(0);
  
  Shader shader("../vert.glsl", "../frag.glsl");
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

