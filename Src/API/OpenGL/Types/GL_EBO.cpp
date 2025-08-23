#include <API/OpenGL/Types/GL_EBO.h>

namespace OpenGLTypes {
  GLEBO::GLEBO(const std::string& name): mEbo(GL_ELEMENT_ARRAY_BUFFER), mName(name){}

  void GLEBO::Bind(){
    mEbo.BindBuffer();
  }

  void GLEBO::Unbind(){
    mEbo.UnbindBuffer();
  }

  void GLEBO::UpdateEbo(GLsizeiptr size, GLenum usage, const void* data){
    mEbo.AllocateAndFillMemory(size, usage, data);
  }
}
