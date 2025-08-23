#include <Types/GL_VBO.h>

namespace OpenGLTypes {
  GLVBO::GLVBO(const std::string& name): mVbo(GL_ARRAY_BUFFER), mName(name) {}

  void GLVBO::Bind(){
    mVbo.BindBuffer();
  }

  void GLVBO::Unbind(){
    mVbo.UnbindBuffer();
  }

  void GLVBO::UpdateVbo(GLsizeiptr size, GLenum usage, const void* data){
    mVbo.AllocateAndFillMemory(size, usage, data); 
  }

  void GLVBO::UpdateVboPartial(GLintptr offset, GLsizeiptr size, const void* data){
    mVbo.FillPartialMemory(offset, size, data);
  }
  
  void GLVBO::UpdateVboNoFill(GLsizeiptr size, GLenum usage){
    mVbo.AllocateMemory(size, usage);
  }

}
