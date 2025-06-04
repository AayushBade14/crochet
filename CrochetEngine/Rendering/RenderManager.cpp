#include "RenderManager.hpp"
#include <vector>
namespace Crochet {
  int divisions = 10;
  float step = 2.0f/divisions;
  std::vector<float> gridLines;

  Platform::WindowManager& mWindowManager = Platform::WindowManager::getInstance();
  Core::Logger& mLogger = Core::Logger::getInstance();

  RenderManager& RenderManager::getInstance(){
    static RenderManager instance = RenderManager();
    return instance; 
  }

  void RenderManager::init(){
    mWindowManager.init();
    mLogger.info("Successfully initialised Crochet WindowManager!");
    mLogger.info("Successfully initialised Crochet RenderManager!");
    Crochet::Graphics::Shader shader = Crochet::Graphics::Shader("./CrochetEngine/Assets/Shaders/vert.glsl","./CrochetEngine/Assets/Shaders/frag.glsl");
    mLogger.info("Successfully initialised Crochet ShaderManager!");
    for(int i = 0;i<=divisions;i++){
      float x = -1.0f + i * step;
      gridLines.push_back(x); gridLines.push_back(-1.0f);
      gridLines.push_back(x); gridLines.push_back(1.0f);
    }

    for(int i = 0;i<=divisions;i++){
      float y = -1.0f + i * step;
      gridLines.push_back(-1.0f); gridLines.push_back(y);
      gridLines.push_back(1.0f);  gridLines.push_back(y);
    }
/*
    float vertices[] = {
      -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
      0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
      0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
    };
*/
    unsigned int VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER,gridLines.size()*sizeof(float),gridLines.data(),GL_STATIC_DRAW);

   // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    
    mLogger.info("Entering Crochet Render Loop!");

    while(!mWindowManager.shouldClose()){
      glClear(GL_COLOR_BUFFER_BIT);
      mWindowManager.pollEvents();

      shader.use();
      glBindVertexArray(VAO);
      //glDrawArrays(GL_TRIANGLES,0,6);
      glDrawArrays(GL_LINES,0,gridLines.size()/2);

      glBindVertexArray(0);

      mWindowManager.swapBuffers();
    }

    mWindowManager.cleanup();
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
  }
}
