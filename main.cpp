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
#include <ctime>

#include "./Vendor/stb_image/stb_image.h"

#include "./World.h"

int Width = 1920;
int Height = 1013;
std::string Title = "crochet-editor";

glm::vec2 camPos = glm::vec2(1.0f);

float dt = 0.0f;
float lastFrame = 0.0f;

float zoom = 1.0f;

GLFWwindow* window = nullptr;

glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);

double xMouseScreen = 0.0f;
double yMouseScreen = 0.0f;

float xMouseWorld = 0.0f;
float yMouseWorld = 0.0f;

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
    else if constexpr(std::is_same_v<T, glm::vec3>) glUniform3fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T, glm::mat4>) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
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

void ProcessInput()
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
    camPos.y += 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
    camPos.y -= 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
    camPos.x -= 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
    camPos.x += 4 * TILE_SIZE * dt;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  zoom -= (float)yoffset * dt * 5.0f;
  if(zoom <= 0.0f)
  {
    zoom = 0.1f;
  }
}


void ScreenToWorld()
{
  float xNDC = (2.0f * xMouseScreen)/(float)Width - 1.0f;
  float yNDC = 1.0f - (2.0f * yMouseScreen)/(float)Height;

  glm::vec4 ray_clip = glm::vec4(xNDC, yNDC, 0.0f, 1.0f);

  glm::vec4 ray_eye = glm::inverse(projection) * ray_clip; 
  ray_eye /= ray_eye.w;

  glm::vec3 ray_world = glm::inverse(view) * ray_eye;

  xMouseWorld = ray_world.x;
  yMouseWorld = ray_world.y;
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
  srand(time(0));

  stbi_set_flip_vertically_on_load(true);
  
  Shader shader("../vert.glsl", "../frag.glsl");

  unsigned int tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char* data  = stbi_load("../test.png", &width, &height, &nrChannels, 0);
  if(data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);
  
  shader.Use();
  shader.SetValue("tex", 0);

  World m_World;

  m_World.Init();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);

  while(!glfwWindowShouldClose(window))
  {
    float currentFrame = (float)glfwGetTime();
    dt = currentFrame -  lastFrame;
    lastFrame = currentFrame;
    
    //std::cout << "FrameRate: " << 1.0f/dt << std::endl;

    glfwPollEvents();
    
    glfwGetCursorPos(window, &xMouseScreen, &yMouseScreen);
    ScreenToWorld();
 
    ProcessInput();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    float aspect = (float)Width/(float)Height;

    glm::mat4 model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-camPos.x, -camPos.y, 0.0f));
    projection = glm::ortho(0.0f, (float)16*TILE_SIZE * aspect * zoom, 0.0f, (float)16*TILE_SIZE * zoom);


    shader.Use();
    shader.SetValue("model", model);
    shader.SetValue("view", view);
    shader.SetValue("projection", projection);

    m_World.Render(window, xMouseWorld, yMouseWorld, camPos.x, camPos.y, zoom, aspect);
    
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
