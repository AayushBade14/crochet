#include <Renderer/Renderer.h>

void Render(){
  
  if(GLFWBackEnd::Init()){
    std::cout<<"GLFW::INIT SUCCESS!"<<std::endl;
  }
  OpenGLBackEnd::Init();
  
  OpenGLTypes::GLShader shader("BasicShader", "./Res/Shaders/vert.glsl","./Res/Shaders/frag.glsl");
  
  OpenGLTypes::GLVAO vao("TriangleVao");
  vao.Bind();

  OpenGLTypes::GLVBO vbo("TriangleVbo");
  vbo.Bind();
  vbo.UpdateVbo(sizeof(vertices), GL_STATIC_DRAW, vertices);
  
  vao.SetAttribPointer(0, 3, 3*sizeof(float), 0);

  vao.Unbind();
  
  OpenGLBackEnd::SetFramebufferSizeCallback();

  while(!GLFWBackEnd::WindowShouldClose()){
    GLFWBackEnd::PollEvents();
    GLFWBackEnd::UpdateTimer();
    
    OpenGLBackEnd::ClearColor(glm::vec4(0.0f,0.0f,0.0f,1.0f),GL_COLOR_BUFFER_BIT);

    shader.Use();
    vao.Draw();

    GLFWBackEnd::SwapBuffers();
  }
  
  GLFWBackEnd::Destroy();
}
