#include "RenderManager.hpp"
#include <vector>
namespace Crochet {

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
    
    std::vector<float> vertices = Core::quadVertices;

    Core::VBO vbo = Core::VBO(vertices);
    Core::VAO vao = Core::VAO(vbo);
    
    vao.setAttribPointer(0,3,11,0);
    vao.setAttribPointer(1,3,11,3);

    mLogger.info("Initialised Buffers!");
    mLogger.info("Entering Crochet Render Loop!");

    while(!mWindowManager.shouldClose()){
      glClear(GL_COLOR_BUFFER_BIT);
      mWindowManager.pollEvents();

      shader.use();
      vao.bind();
      glDrawArrays(GL_TRIANGLES,0,vertices.size()/2);

      vao.unbind();
      
      mWindowManager.swapBuffers();
    }
    
    vao.cleanup();
    vbo.cleanup();
    mWindowManager.cleanup();
  }
}
