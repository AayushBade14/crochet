#include <Renderer/Renderer.h>

float vertices[] = {
  -0.05f, -0.05f, 0.0f,
  0.05f, -0.05f,  0.0f,
  0.0f, 0.05f, 0.0f
};

glm::vec3 offsets[100];
int id = 0;

void Render(){
  
  if(GLFWBackEnd::Init()){
    std::cout<<"GLFW::INIT SUCCESS!"<<std::endl;
  }
  OpenGLBackEnd::Init();
 
  for(int i = -10; i < 10; i+=2){
    for(int j = -10; j < 10; j+=2){
      glm::vec3 translation;
      translation.x = (float)i/10.0f + 0.1f;
      translation.y = (float)j/10.0f + 0.1f;
      translation.z = 0.0f;
      offsets[id++] = translation;
    }
  }

  OpenGLTypes::GLShader shader("BasicShader", "./Res/Shaders/vert.glsl", "./Res/Shaders/frag.glsl");
  
  OpenGLTypes::GLVAO vao("TriangleVao");
  vao.Bind();

  OpenGLTypes::GLVBO vbo("TriangleVbo");
  vbo.Bind();
  vbo.UpdateVbo(sizeof(vertices), GL_STATIC_DRAW, vertices);
  
  vao.SetAttribPointer(0, 3, 3*sizeof(float), 0);
  
  OpenGLTypes::GLVBO offsetVbo("TriangleOffsetVbo");
  offsetVbo.Bind();
  offsetVbo.UpdateVbo(100*sizeof(glm::vec3), GL_STATIC_DRAW, offsets);

  vao.SetAttribPointer(1, 3, sizeof(glm::vec3), 0);

  offsetVbo.Unbind();
  
  vao.SetAttribDivisor(1,1);
  
  vao.Unbind();
  
  OpenGLBackEnd::SetFramebufferSizeCallback();

  while(!GLFWBackEnd::WindowShouldClose()){
    GLFWBackEnd::PollEvents();
    GLFWBackEnd::UpdateTimer();
    
    OpenGLBackEnd::ClearColor(glm::vec4(0.0f,0.0f,0.0f,1.0f),GL_COLOR_BUFFER_BIT);

    shader.Use();
    vao.InstancedDraw(GL_TRIANGLES, 0, 3, 100);

    GLFWBackEnd::SwapBuffers();
  }
  
  GLFWBackEnd::Destroy();
}
