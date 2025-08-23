#include <API/OpenGL/Types/GL_Shader.h>

namespace OpenGLTypes {
  GLShader::GLShader(const std::string& name, const std::string& vertPath, const std::string& fragPath): mName(name), mVertPath(vertPath), mFragPath(fragPath) {
    std::string vertCode = LoadFile(vertPath);
    std::string fragCode = LoadFile(fragPath);

    GLuint vert = CompileShader(vertCode, ShaderType::VERT);
    GLuint frag = CompileShader(fragCode, ShaderType::FRAG);

    CreateShaderProgram(vert, frag);

    std::cout<<mName<<" Successfully Created!"<<std::endl;
  }

  GLShader::GLShader(const std::string& name, const std::string& vertPath, const std::string& geomPath, const std::string& fragPath): mName(name), mVertPath(vertPath), mGeomPath(geomPath), mFragPath(fragPath){
    std::string vertCode = LoadFile(vertPath);
    std::string geomCode = LoadFile(geomPath);
    std::string fragCode = LoadFile(fragPath);

    GLuint vert = CompileShader(vertCode, ShaderType::VERT);
    GLuint geom = CompileShader(geomCode, ShaderType::GEOM);
    GLuint frag = CompileShader(fragCode, ShaderType::FRAG);

    CreateShaderProgram(vert, geom, frag);
    
    std::cout<<mName<<" Successfully Created!"<<std::endl;
  }

  GLShader::~GLShader(){
    glDeleteProgram(mId);
    std::cout<<mName<<" Resources Cleaned!"<<std::endl;  
  }

  std::string GLShader::LoadFile(const std::string& path){
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
    catch(const std::ifstream::failure &e){
      std::cout<<"ERROR: Reading Shader File ["<<path<<"]: "<<e.what()<<std::endl;
    }

    return code;
  }

  GLuint GLShader::CompileShader(const std::string& srcCode, GLShaderType type){
    int success;
    char infoLog[512];
    GLuint shader;
    GLchar* code = srcCode.c_str();
    std::string tname;

    switch (type) {
      case GLShaderType::VERT:
        tname = "VERTEX";
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
      case GLShaderType::GEOM:
        tname = "GEOMETRY";
        shader = glCreateShader(GL_GEOMETRY_SHADER);
        break;
      case GLShaderType::FRAG:
        tname = "FRAGMENT";
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
      default:
        std::cout<<"ERROR: Not a valid shader-type!"<<std::endl;
    }

    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout<<"ERROR: Shader::Compile("<<mName<<") "<<"-> "<<tname<<" Shader -> "<<infoLog<<std::endl;
    }
    
    std::cout<<"ERROR: Shader::Compile("<<mName<<") - "<<tname<<" Shader Successful!"<<std::endl;
    return shader;
  }
  
  void GLShader::CreateShaderProgram(GLuint& vert, GLuint& frag){
    mId = glCreateProgram();
    glAttachShader(mId, vert);
    glAttachShader(mId, frag);

    int success;
    char infoLog[512];

    glLinkProgram(mId);
    
    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(mId, 512, NULL, infoLog);
      std::cout<<"ERROR: Linking::ShaderProgram("<<mName<<") -> "<<infoLog<<std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    
    std::cout<<"Linking::ShaderProgram("<<mName<<") "<<"Successful!"<<std::endl;
  }
  
  void GLShader::CreateShaderProgram(GLuint& vert, GLuint& geom, GLuint& frag){
    mId = glCreateProgram();
    glAttachShader(mId, vert);
    glAttachShader(mId, geom);
    glAttachShader(mId, frag);

    int success;
    char infoLog[512];

    glLinkProgram(mId);

    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(mId, 512, NULL, infoLog);
      std::cout<<"ERROR: Linking::ShaderProgram("<<mName<<") -> "<<infoLog<<std::endl;
    }
    
    glDeleteShader(vert);
    glDeleteShader(geom);
    glDeleteShader(frag);

    std::cout<<"Linking::ShaderProgram("<<mName<<") "<<"Successful!"<<std::endl;
  }

  void GLShader::Use(){
    glUseProgram(mId);
    std::cout<<"IN-USE: "<<mName<<std::endl;
  }
}
