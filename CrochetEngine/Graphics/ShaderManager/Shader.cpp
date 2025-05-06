#include "Shader.hpp"

namespace Crochet::Graphics {
  Crochet::Core::Logger& mLogger = Crochet::Core::Logger::getInstance();

  const char* loadFile(std::string &path){
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
      file.open(path);
      
      std::stringstream stream;
      
      ss << file.rdbuf();
      
      file.close();
      
      code = stream.str();
    }
    catch(const std::ifstream::failure &e){
      mLogger.warn("Failed to load file contents");
    }

    return code.c_str();
  }
  
  unsigned int Shader::compileShader(unsigned int type,const char *source){
    unsigned int shader;
    int success;
    char infoLog[512];

    if(type == 0){
      shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else{
      shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shader,1,&source,NULL);
    glCompileShader(shader);

    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
      glGetShaderInfoLog(shader,512,NULL,infoLog);
      mLogger.warn(infoLog);
    }
    else{
      mLogger.info("Shader compiled successfully!");
    }
    
    return shader;
  }
      
  void Shader::linkProgram(unsigned int &vertexShader,unsigned int &fragmentShader){
    int success;
    char infoLog[512];

    mProgramID = glCreateProgram();
    glAttachShader(mProgramID,vertexShader);
    glAttachShader(mProgramID,fragmentShader);
    glLinkProgram(mProgramID);

    glGetProgramiv(mProgramID,GL_LINK_STATUS,&success);
    if(!success){
      glGetProgramInfoLog(mProgramID,512,NULL,infoLog);
      mLogger.warn(infoLog);
    }
    else{
      mLogger.info("Shader program successfully linked!");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  } 

  Shader::Shader(const std::string &vertexPath,const std::string &fragmentPath){
    const char* vertexSrc = loadFile(vertexPath);
    const char* fragmentSrc = loadFile(fragmentPath);

    unsigned int vertex = compileShader(0,vertexSrc);
    unsigned int fragment = compileShader(1,fragmentSrc);

    linkProgram(vertex,fragment);
  }

  Shader::~Shader(){
    glDeleteProgram(mProgramID);
  }

  void Shader::use(){
    glUseProgram(mProgramID);
  }

  void Shader::setBool(const std::string& name,bool value) const{
    unsigned int loc = glGetUniformLocation(mProgramID,name.c_str());
    glUniform1i(loc,(int)value);
  }

  void Shader::setInt(const std::string& name,int value) const{
    unsigned int loc = glGetUniformLocation(mProgramID,name.c_str());
    glUniform1i(loc,value);
  }

  void Shader::setFloat(const std::string& name,float value) const{
    unsigned int loc = glGetUniformLocation(mProgramID,name.c_str());
    glUniform1f(loc,value);
  }

  void Shader::setVec2(const std::string& name,float x,float y) const{
    unsigned int loc = glGetUniformLocation(mProgramID,name.c_str());
    glUniform2f(loc,x,y);
  }

  void Shader::setVec2(const std::string& name,glm::vec2 &vector) const{
    unsigned int loc = glGetUniformLocation(mProgramID,name.c_str());
    glUniform2fv(loc,1,&vector[0]);
  }

  void Shader::setVec3(const std::string& name,float x,float y,float z) const{
    
  }
}
