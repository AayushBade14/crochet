#include <Include/OGL.h>
#include <GLFW/glfw3.h>

#define WIDTH 1920
#define HEIGHT 1013
#define TITLE "Crochet-Engine"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0,0,width,height);
}

void ProcessInput(GLFWwindow* window){
  if(glfwGetKey())
}

int main(void){
  if(glfwInit()<0){
    std::cerr<<"ERROR: GLFW::INIT()!"<<std::endl;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,TITLE,NULL,NULL);

  if(!window){
    std::cerr<<"ERROR: Window::Create()!"<<std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    glfwTerminate();
    glfwDestroyWindow(window);
    std::cerr<<"ERROR: GLAD::INIT()!"<<std::endl;
  }

  const float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.0f, 0.5f,0.0f,  0.0f, 0.0f, 1.0f,   0.5f, 1.0f
  }; 
  
  Shader shader("TriangleShader","../Assets/vert.glsl","../Assets/frag.glsl");
  
  Texture2D texture("BrickTexture");
  texture.Bind();
  texture.SetWrapS(GL_REPEAT);
  texture.SetWrapS(GL_REPEAT);
  texture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
  texture.SetMagFilter(GL_LINEAR);
  texture.LoadTexture("../Assets/brick.jpg", true);
  texture.Unbind();

  texture.SetSamplerValue(shader, "tex", 0);
  
  VAO vao("TriangleVao");
  VBO vbo("TriangleVbo");

  vao.Bind();
  vbo.Bind();
  vbo.AllocateAndFillMemory(GL_STATIC_DRAW, sizeof(vertices), vertices);
  vao.SetAttribPointer(0,3,8,0);
  vao.SetAttribPointer(1,3,8,3);
  vao.SetAttribPointer(2,2,8,6);
  vao.Unbind();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    texture.ActivateTextureUnit(0);
    texture.Bind();

    shader.Use();

    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.Unbind();
    
    texture.Unbind();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  glfwDestroyWindow(window);
  return 0;
}
