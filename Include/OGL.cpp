#include <Include/OGL.h>

Shader::Shader(const std::string& name, const std::string& vertPath, const std::string& fragPath): mName(name){
  std::string vertCode = LoadFile(vertPath);
  std::string fragCode = LoadFile(fragPath);
  unsigned int vert = CompileShader(vertCode, ShaderType::VERT);
  unsigned int frag = CompileShader(fragCode, ShaderType::FRAG);
  CreateShaderProgram(vert, frag);
}

Shader::Shader(const std::string& name, const std::string& vertPath, const std::string& geomPath, const std::string& fragPath): mName(name){
  std::string vertCode = LoadFile(vertPath);
  std::string geomCode = LoadFile(geomPath);
  std::string fragCode = LoadFile(fragPath);
  unsigned int vert = CompileShader(vertCode, ShaderType::VERT);
  unsigned int geom = CompileShader(geomCode, ShaderType::GEOM);
  unsigned int frag = CompileShader(fragCode, ShaderType::FRAG);
  CreateShaderProgram(vert, geom, frag);
}

Shader::~Shader(){
  glDeleteProgram(mId);
}

void Shader::Use(){
  glUseProgram(mId);
}

std::string Shader::LoadFile(const std::string& path){
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
  catch(const std::ifstream::failure& e){
    std::cerr<<"ERROR: Reading file -> "<<e.what()<<std::endl;
  }

  return code;
}

unsigned int Shader::CompileShader(const std::string& srcCode, ShaderType type){
  const char* code = srcCode.c_str();
  int success;
  char infoLog[512];
  std::string stype;
  unsigned int shader;

  switch (type) {
    case ShaderType::VERT:
      stype = "Vertex";
      shader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case ShaderType::GEOM:
      stype = "Geometry";
      shader = glCreateShader(GL_GEOMETRY_SHADER);
      break;
    case ShaderType::FRAG:
      stype = "Fragment";
      shader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      std::cerr<<"ERROR: Not a known shader type!"<<std::endl;
      break;
  }

  glShaderSource(shader,1,&code,NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader,512,NULL,infoLog);
    std::cerr<<"ERROR["<<mName<<"]: Error compiling "<<stype<<" shader! -> "<<infoLog<<std::endl;
  }
  else{
    std::cout<<"Success["<<mName<<"]: compiling "<<stype<<" shader!"<<std::endl;
  }

  return shader;
}


void Shader::CreateShaderProgram(const unsigned int& vert, const unsigned int& frag){
  int success;
  char infoLog[512];

  mId = glCreateProgram();
  glAttachShader(mId, vert);
  glAttachShader(mId, frag);
  glLinkProgram(mId);

  glGetProgramiv(mId, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(mId, 512, NULL, infoLog);
    std::cerr<<"ERROR["<<mName<<"]: linking shader program!"<<std::endl;
  }
  else{
    std::cout<<"Success["<<mName<<"]: creating shader program!"<<std::endl;
  }
  
  glDeleteShader(vert);
  glDeleteShader(frag);
}

void Shader::CreateShaderProgram(const unsigned int& vert, const unsigned int& geom, const unsigned int& frag){
  int success;
  char infoLog[512];

  mId = glCreateProgram();
  glAttachShader(mId, vert);
  glAttachShader(mId, geom);
  glAttachShader(mId, frag);
  glLinkProgram(mId);

  glGetProgramiv(mId, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(mId, 512, NULL, infoLog);
    std::cerr<<"ERROR["<<mName<<"]: linking shader program!"<<std::endl;
  }
  else{
    std::cout<<"Success["<<mName<<"]: creating shader program!"<<std::endl;
  }
  
  glDeleteShader(vert);
  glDeleteShader(geom);
  glDeleteShader(frag);
}


