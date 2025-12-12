#define GLM_EXPERIMENTAL_ENABLED

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#define MAX_BONE_INFLUENCE 4
#define MAX_BONES 100

glm::vec2 AiToGlm(const aiVector2D& v) {return glm::vec2(v.x, v.y);}
glm::vec3 AiToGlm(const aiVector3D& v) {return glm::vec3(v.x, v.y, v.z);}
glm::quat AiToGlm(const aiQuaternion& v) {return glm::quat(v.w, v.x, v.y, v.z);}
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

  void SetFinalBoneMatrices(const std::vector<glm::mat4>& finalMatrix){
    Use();
    for(unsigned int i = 0; i < finalMatrix.size(); i++){
      std::string nr = std::to_string(i);
      std::string name = "uBones["+nr+"]";
      SetValue(name, finalMatrix[i]);
    }
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
  
  int mBoneIds[MAX_BONE_INFLUENCE];
  float mWeights[MAX_BONE_INFLUENCE];

  Vertex(){
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
      mBoneIds[i] = -1;
      mWeights[i] = 0.0f;
    }
  }
};

struct Bone{
  std::string mBoneName;

  int mParentIndex = -1;
  std::vector<int> mChildrenIndices;

  glm::mat4 mOffsetMatrix = glm::mat4(1.0f);
  glm::mat4 mLocalTransform = glm::mat4(1.0f);
  glm::mat4 mGlobalTransform = glm::mat4(1.0f);
};

struct Skeleton{
  std::vector<Bone> mBones;
  std::unordered_map<std::string, int> mBoneInfoMap;
  
  void UpdateHierarchy(int index, const glm::mat4& parentTransform){
    Bone& bone = mBones[index];
    bone.mGlobalTransform = parentTransform * bone.mLocalTransform;

    for(auto& child : bone.mChildrenIndices){
      UpdateHierarchy(child, bone.mGlobalTransform);
    }
  }

  std::vector<glm::mat4> GetFinalBoneMatrices(){
    std::vector<glm::mat4> finalMatrix(mBones.size());
    for(unsigned int i = 0; i < mBones.size(); i++){
      finalMatrix[i] = mBones[i].mGlobalTransform * mBones[i].mOffsetMatrix;
    }
    return finalMatrix;
  }
};

struct KeyVec3{
  float mTime;
  glm::vec3 mValue;
};

struct KeyQuat{
  float mTime;
  glm::quat mValue;
};

struct BoneTrack{
  std::string mBonetrackName;

  int mNumPosKeys;
  int mNumRotKeys;
  int mNumScaleKeys;

  std::vector<KeyVec3> mPosKeys;
  std::vector<KeyQuat> mRotKeys;
  std::vector<KeyVec3> mScaleKeys;
};

struct AnimationClip{
  std::string mAnimationClipName;

  float mDuration;
  float mTicksPerSecond;

  std::vector<BoneTrack> mBoneTracks;
};

struct Animator{
  Skeleton* mSkeleton = nullptr;
  AnimationClip* mAnimationClip = nullptr;

  bool mIsLooped = true;

  float mCurrentTime = 0.0f;

  void Update(float dt){
    if(!mSkeleton || !mAnimationClip){
      std::cerr<<"Error: No skeleton/animation-clip found!"<<std::endl;
      exit(1);
    }
    
    mCurrentTime += mAnimationClip->mTicksPerSecond * dt;

    if(mIsLooped) mCurrentTime = fmod(mCurrentTime, mAnimationClip->mDuration);

    ApplyAnimation();
  }

  void ApplyAnimation(){
    for(auto& bonetrack : mAnimationClip->mBoneTracks){
      Bone& bone = mSkeleton->mBones[mSkeleton->mBoneInfoMap[bonetrack.mBonetrackName]];

      glm::vec3 pos = InterpolateVec3(bonetrack.mPosKeys, mCurrentTime);
      glm::quat rot = InterpolateQuat(bonetrack.mRotKeys, mCurrentTime);
      glm::vec3 scale = InterpolateVec3(bonetrack.mScaleKeys, mCurrentTime);

      glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
      glm::mat4 R = glm::toMat4(rot);
      glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

      bone.mLocalTransform = T * R * S;
    }

    mSkeleton->UpdateHierarchy(0, glm::mat4(1.0f));
  }

private:

  glm::vec3 InterpolateVec3(const std::vector<KeyVec3>& keys, float dt){
    if(keys.empty()) return glm::vec3(1.0f);
    if(keys.size() == 1) return keys[0].mValue;

    for(unsigned int i = 0; i < keys.size() - 1; i++){
      if(dt >= keys[i].mTime && dt <= keys[i+1].mTime){
        float t = (dt - keys[i].mTime)/(keys[i+1].mTime - keys[i].mTime);
        return glm::mix(keys[i].mValue, keys[i+1].mValue, t);
      }
    }

    return keys.back().mValue;
  }

  glm::quat InterpolateQuat(const std::vector<KeyQuat>& keys, float dt){
    if(keys.empty()) return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if(keys.size() == 1) return keys[0].mValue;

    for(unsigned int i = 0; i < keys.size() - 1; i++){
      if(dt >= keys[i].mTime && dt <= keys[i+1].mTime){
        float t = (dt - keys[i].mTime)/(keys[i+1].mTime - keys[i].mTime);
        return glm::slerp(keys[i].mValue, keys[i+1].mValue, t);
      }
    }

    return keys.back().mValue;
  }
 
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
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, mBoneIds));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mWeights));

    glBindVertexArray(0);
  }
};

struct Model{
  Skeleton mSkeleton;
  Animator mAnimator;
  std::vector<AnimationClip> mAnimationClips;

  std::vector<Mesh*> mModelMeshes;

  Model(const std::string& path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
      std::cerr<<importer.GetErrorString()<<std::endl;
      exit(1);
    }
    
    ProcessBones(scene->mRootNode, -1);
    ProcessAnimation(scene);
    ProcessNode(scene->mRootNode, scene);
    
    if(!mAnimationClips.empty()) mAnimator.mAnimationClip = &mAnimationClips[0];
    mAnimator.mSkeleton = &mSkeleton;

    std::cout<<"Loaded: "<<mAnimator.mAnimationClip->mAnimationClipName<<std::endl;
    std::cout<<"Loaded: "<<mSkeleton.mBones.size()<<" bones"<<std::endl;
    std::cout<<"Model loaded successfully!"<<std::endl;
  }

  ~Model(){
    for(auto* mesh : mModelMeshes)
      delete mesh;
  }
  
  void PlayAnimation(float dt){
    mAnimator.Update(dt);
  }
  
  void Draw(Shader& shader){
    shader.SetFinalBoneMatrices(mSkeleton.GetFinalBoneMatrices());
    for(auto* mesh : mModelMeshes)
      mesh->Draw(shader);
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

  Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene){
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
    
    ExtractBoneWeights(mesh, vertices);
    
    for(unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++){
      aiFace face = mesh->mFaces[faceIndex];
      for(unsigned int index = 0; index < face.mNumIndices; index++){
        indices.push_back(face.mIndices[index]);
      }
    }

    return new Mesh(vertices, indices);
  }

  void ProcessAnimation(const aiScene* scene){
    for(unsigned int i = 0; i < scene->mNumAnimations; i++){
      aiAnimation* animation = scene->mAnimations[i];
      
      AnimationClip clip;
      clip.mAnimationClipName = animation->mName.C_Str();
      clip.mDuration = animation->mDuration;
      clip.mTicksPerSecond = animation->mTicksPerSecond;
      clip.mBoneTracks = ProcessBonetracks(animation);
      
      mAnimationClips.push_back(clip);
    }
  }

  std::vector<BoneTrack> ProcessBonetracks(aiAnimation* animation){
    std::vector<BoneTrack> bonetracks;

    for(unsigned int i = 0; i < animation->mNumChannels; i++){
      aiNodeAnim* nodeanim = animation->mChannels[i];
      
      BoneTrack bonetrack;
      bonetrack.mBonetrackName = nodeanim->mNodeName.C_Str();
      bonetrack.mNumPosKeys = nodeanim->mNumPositionKeys;
      bonetrack.mNumRotKeys = nodeanim->mNumRotationKeys;
      bonetrack.mNumScaleKeys = nodeanim->mNumScalingKeys;

      for(int j = 0; j < bonetrack.mNumPosKeys; j++){
        KeyVec3 key;
        key.mTime = nodeanim->mPositionKeys[j].mTime;
        key.mValue = AiToGlm(nodeanim->mPositionKeys[j].mValue);
        
        bonetrack.mPosKeys.push_back(key);
      }

      for(int j = 0; j < bonetrack.mNumRotKeys; j++){
        KeyQuat key;
        key.mTime = nodeanim->mRotationKeys[j].mTime;
        key.mValue = AiToGlm(nodeanim->mRotationKeys[j].mValue);
        
        bonetrack.mRotKeys.push_back(key);
      }

      for(int j = 0; j < bonetrack.mNumScaleKeys; j++){
        KeyVec3 key;
        key.mTime = nodeanim->mScalingKeys[j].mTime;
        key.mValue = AiToGlm(nodeanim->mScalingKeys[j].mValue);
        
        bonetrack.mScaleKeys.push_back(key);
      }
      
      bonetracks.push_back(bonetrack);
    }
    return bonetracks;
  }

  void ExtractBoneWeights(aiMesh* mesh, std::vector<Vertex>& vertices){
    for(unsigned int i = 0; i < mesh->mNumBones; i++){
      aiBone* bone = mesh->mBones[i];
      std::string bonename = bone->mName.C_Str();

      if(mSkeleton.mBoneInfoMap.find(bonename) != mSkeleton.mBoneInfoMap.end()){
        int boneId = mSkeleton.mBoneInfoMap[bonename];
        mSkeleton.mBones[boneId].mOffsetMatrix = AiToGlm(bone->mOffsetMatrix);

        for(unsigned int j = 0; j < bone->mNumWeights; j++){
          float weight = bone->mWeights[j].mWeight;
          int vertexId = bone->mWeights[j].mVertexId;

          for(unsigned int k = 0; k < MAX_BONE_INFLUENCE; k++){
            if(vertices[vertexId].mBoneIds[k] < 0){
              vertices[vertexId].mBoneIds[k] = boneId;
              vertices[vertexId].mWeights[k] = weight;
              break;
            }
          }
        }
      }
    }
  }

  void ProcessBones(aiNode* node, int parentIndex){
      int currentIndex = mSkeleton.mBones.size();

      Bone bone;
      bone.mBoneName = node->mName.C_Str();
      bone.mParentIndex = parentIndex;
      if(parentIndex >= 0) mSkeleton.mBones[parentIndex].mChildrenIndices.push_back(currentIndex);
      bone.mOffsetMatrix = glm::mat4(1.0f);
      bone.mLocalTransform = AiToGlm(node->mTransformation);
      bone.mGlobalTransform = glm::mat4(1.0f);
    
      mSkeleton.mBoneInfoMap[bone.mBoneName] = currentIndex;
      mSkeleton.mBones.push_back(bone);

      for(unsigned int i = 0; i < node->mNumChildren; i++){
        ProcessBones(node->mChildren[i], currentIndex);
      }
  }
};

int WIDTH = 1920;
int HEIGHT = 1080;
const char* TITLE = "crochet";

GLFWwindow* window = nullptr;

float lastFrame = 0.0f;
float dt = 0.0f;

void UpdateTimer(){
  float currentFrame = (float)glfwGetTime();
  dt = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void ProcessInput(){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

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
  Model player("../Dancing.dae");
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  glEnable(GL_DEPTH_TEST);
  
  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();
    ProcessInput();

    UpdateTimer();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    float radius = 6.0f;
    float camX = sin(glfwGetTime() * 0.5f) * radius;
    float camZ = cos(glfwGetTime() * 0.5f) * radius;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(camX, 2.0f, camZ), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f);
    
    shader.Use();
    shader.SetValue("model", model);
    shader.SetValue("view", view);
    shader.SetValue("projection", projection);
    
    player.PlayAnimation(dt);
    player.Draw(shader);
    
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

