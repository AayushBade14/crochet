#include "RenderManager.hpp"

namespace Crochet {
  
  Platform::WindowManager& mWindowManager = Platform::WindowManager::getInstance();

  RenderManager& RenderManager::getInstance(){
    static RenderManager instance = RenderManager();
    return instance; 
  }

  void RenderManager::init(){
    mWindowManager.init();
    
    Crochet::Graphics::Shader shader = Crochet::Graphics::Shader("vert.glsl","frag.glsl");

    float vertices[] = {
      -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
      0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
      0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
    };

    unsigned int VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);

    while(!mWindowManager.shouldClose()){
      glClear(GL_COLOR_BUFFER_BIT);
      mWindowManager.pollEvents();

      shader.use();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES,0,6);

      glBindVertexArray(0);

      mWindowManager.swapBuffers();
    }

    mWindowManager.cleanup();
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
  }
}
