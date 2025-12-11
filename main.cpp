#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

glm::vec2 AiToGlm(const aiVector2D& v) {return glm::vec2(v.x, v.y);}
glm::vec3 AiToGlm(const aiVector3D& v) {return glm::vec3(v.x, v.y, v.z);}
glm::mat4 AiToGlm(const aiMatrix4x4& m) {return glm::transpose(glm::mat4(
  m.a1, m.a2, m.a3, m.a4,
  m.b1, m.b2, m.b3, m.b4,
  m.c1, m.c2, m.c3, m.c4,
  m.d1, m.d2, m.d3, m.d4
));}

struct Shader{
  unsigned int mId;

  Shader(const std::string& vertPath, const std::string& fragPath){
    std::string vCode = LoadFile(vertPath);
    std::string fCode = LoadFile(fragPath);
    unsigned int vert = CompileShader(vCode, true);
    unsigned int frag = CompileShader(fCode, false);
    CreateShaderProgram(vert, frag);
  }

  ~Shader() {glDeleteProgram(mId);}

  void Use() {glUseProgram(mId);}

  template <typename T>
  void SetValue(const std::string& name, const T& val){
    const unsigned int loc = glGetUniformLocation(mId, name.c_str());
  
    if constexpr(std::is_same_v<T,int>) glUniform1i(loc, val);
    else if constexpr(std::is_same_v<T,bool>) glUniform1i(loc, (int)val);
    else if constexpr(std::is_same_v<T,float>) glUniform1f(loc, val);
    else if constexpr(std::is_same_v<T,glm::vec2>) glUniform2fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T,glm::vec3>) glUniform3fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T,glm::mat4>) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  }

private:
  std::string LoadFile(const std::string& path){
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
      exit(1);
    }

    return code;
  }

  unsigned int CompileShader(const std::string& srcCode, bool isVert){
    const char* code = srcCode.c_str();
    int success;
    char infoLog[512];
    unsigned int shader = isVert? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr<<infoLog<<std::endl;
      exit(1);
    }

    return shader;
  }

  void CreateShaderProgram(unsigned int& vert, unsigned int& frag){
    mId = glCreateProgram();
    glAttachShader(mId, vert);
    glAttachShader(mId, frag);
    glLinkProgram(mId);

    int success;
    char infoLog[512];

    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if(!success){
      glGetProgramInfoLog(mId, 512, nullptr, infoLog);
      std::cerr<<infoLog<<std::endl;
      exit(1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
  }
};

struct Vertex{
  glm::vec3 mPosition;
  glm::vec3 mNormal;
  glm::vec2 mTexCoords;
};

struct Mesh{
  unsigned int mVbo;
  unsigned int mEbo;
  unsigned int mVao;

  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;
  
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices){
    mVertices = vertices;
    mIndices = indices;
    SetupMesh();
  }

  ~Mesh() = default;

  void Draw(Shader& shader){
    shader.Use();
    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:

  void SetupMesh(){
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);

    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));

    glBindVertexArray(0);
  }
};

struct Model{
  std::vector<Mesh> mModelMeshes;

  Model(const std::string& path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE | !scene->mRootNode){
      std::cerr<<importer.GetErrorString()<<std::endl;
      exit(1);
    }

    ProcessNode(scene->mRootNode, scene);
  }

  ~Model() = default;

  void Draw(Shader& shader){
    for(auto& mesh : mModelMeshes)
      mesh.Draw(shader);
  }

private:
  void ProcessNode(aiNode* node, const aiScene* scene){
    for(unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++){
      aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];
      mModelMeshes.push_back(ProcessMesh(mesh, scene));
    }

    for(unsigned int childIndex = 0; childIndex < node->mNumChildren; childIndex++){
      ProcessNode(node->mChildren[childIndex], scene);
    }
  }

  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for(unsigned int vertIndex = 0; vertIndex < mesh->mNumVertices; vertIndex++){
      Vertex v;

      v.mPosition = AiToGlm(mesh->mVertices[vertIndex]);
      if(mesh->HasNormals())
        v.mNormal = AiToGlm(mesh->mNormals[vertIndex]);
      if(mesh->HasTextureCoords(0))
        v.mTexCoords = AiToGlm(mesh->mTextureCoords[0][vertIndex]);
      
      vertices.push_back(v);
    }

    for(unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++){
      aiFace face = mesh->mFaces[faceIndex];
      for(unsigned int index = 0; index < face.mNumIndices; index++){
        indices.push_back(face.mIndices[index]);
      }
    }

    return Mesh(vertices, indices);
  }
};

int WIDTH = 1920;
int HEIGHT = 1013;
const char* TITLE = "crochet";

GLFWwindow* window = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

int main(void){
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WIDTH,
                            HEIGHT,
                            TITLE,
                            nullptr,
                            nullptr);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  
  Shader shader("../vert.glsl", "../frag.glsl");
  Model player("../monkey.obj");
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  glEnable(GL_DEPTH_TEST);
  
  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 5.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f);
    
    shader.Use();
    shader.SetValue("model", model);
    shader.SetValue("view", view);
    shader.SetValue("projection", projection);

    player.Draw(shader);
    
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

