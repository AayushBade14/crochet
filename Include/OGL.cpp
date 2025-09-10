#include <Include/OGL.h>
//===============[Shader]=====================
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


//=====================[VBO]==========================

VBO::VBO(const std::string& name): mName(name), mType(GL_ARRAY_BUFFER){
  glGenBuffers(1,&mId);
}

VBO::~VBO(){
  glDeleteBuffers(1,&mId);
}

void VBO::Bind(){
  glBindBuffer(mType, mId);
}

void VBO::Unbind(){
  glBindBuffer(mType, 0);
}

void VBO::AllocateAndFillMemory(GLenum usage, GLsizeiptr size, const void* data){
  glBufferData(mType, size, data, usage);
}

void VBO::AllocateMemory(GLenum usage, GLsizeiptr size){
  glBufferData(mType, size, NULL, usage);
}

void VBO::FillPartialMemory(GLsizeiptr size, GLintptr offset, const void* data){
  glBufferSubData(mType, offset, size, data);
}

//============================[VAO]==================================
VAO::VAO(const std::string& name): mName(name){
  glGenVertexArrays(1,&mId);
}

VAO::~VAO(){
  glDeleteVertexArrays(1,&mId);
}

void VAO::Bind(){
  glBindVertexArray(mId);
}

void VAO::Unbind(){
  glBindVertexArray(0);
}

void VAO::SetAttribPointer(int loc, int nrvals, int stride, int start){
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, nrvals, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(start*sizeof(float)));
}

//===========================[TEXTURE-2D]=================================
Texture2D::Texture2D(const std::string& name): mName(name), mType(GL_TEXTURE_2D) {
  glGenTextures(1,&mId);
}

Texture2D::~Texture2D(){
  glDeleteTextures(1,&mId);
}

void Texture2D::LoadTexture(const std::string& path, bool generateMipmap){
  stbi_set_flip_vertically_on_load(true);
  
  int width;
  int height;
  int nrChannels;

  GLenum flag = GL_RGB;

  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  
  switch (nrChannels) {
    case 1:
      flag = GL_RED;
      break;
    case 3:
      flag = GL_RGB;
      break;
    case 4:
      flag = GL_RGBA;
      break;
  }

  if(data){
    glTexImage2D(mType, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
    if(generateMipmap)
      glGenerateMipmap(mType);
  }

  stbi_image_free(data);
}

void Texture2D::Bind(){
  glBindTexture(mType, mId);
}

void Texture2D::Unbind(){
  glBindTexture(mType, 0);
}

void Texture2D::SetWrapS(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_S, value);
}

void Texture2D::SetWrapT(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_T, value);
}

void Texture2D::SetWrapR(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_R, value);
}


void Texture2D::SetMinFilter(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, value);
}

void Texture2D::SetMagFilter(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, value);
}

void Texture2D::SetSamplerValue(Shader& shader, const std::string& name, int value){
  shader.Use();
  shader.SetValue(name, value);
}

void Texture2D::ActivateTextureUnit(int value){
  glActiveTexture(GL_TEXTURE0 + value);
}

//================================[TEXTURE-3D]======================================

Texture3D::Texture3D(const std::string& name): mName(name), mType(GL_TEXTURE_CUBE_MAP){
  glGenTextures(1,&mId);
}

Texture3D::~Texture3D(){
  glDeleteTextures(1,&mId);
}

void Texture3D::LoadTexture(const std::vector<std::string>& paths){
  stbi_set_flip_vertically_on_load(false);
  
  int width;
  int height;
  int nrChannels;
  GLenum flag = GL_RGB;

  for(unsigned int i = 0; i < paths.size(); i++){
    std::cout<<"Loading texture "<<paths[i]<<std::endl;

    unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
    switch (nrChannels) {
      case 1:
        flag = GL_RED;
        break;
      case 3:
        flag = GL_RGB;
        break;
      case 4:
        flag = GL_RGBA;
        break;
    }

    if(data){
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
    }
    else{
      std::cerr<<"ERROR: Loading Texture::"<<paths[i]<<std::endl;
    }

    stbi_image_free(data);
  }

  SetWrapS(GL_CLAMP_TO_EDGE);
  SetWrapT(GL_CLAMP_TO_EDGE);
  SetWrapR(GL_CLAMP_TO_EDGE);
  SetMinFilter(GL_LINEAR);
  SetMagFilter(GL_LINEAR);
}

void Texture3D::SetWrapS(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_S, value);
}

void Texture3D::SetWrapT(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_T, value);
}

void Texture3D::SetWrapR(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_WRAP_R, value);
}


void Texture3D::SetMinFilter(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_MIN_FILTER, value);
}

void Texture3D::SetMagFilter(GLenum value){
  glTexParameteri(mType, GL_TEXTURE_MAG_FILTER, value);
}

void Texture3D::SetSamplerValue(Shader& shader, const std::string& name, int value){
  shader.Use();
  shader.SetValue(name, value);
}

void Texture3D::ActivateTextureUnit(int value){
  glActiveTexture(GL_TEXTURE0 + value);
}


