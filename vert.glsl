#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;
layout(location=3)in ivec4 aBoneIds;
layout(location=4)in vec4 aWeights;

out vec3 Normal;

const int MAX_BONE_INFLUENCE = 4;
const int MAX_BONES = 100;

uniform mat4 uBones[MAX_BONES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
  mat4 transform = mat4(0.0);

  for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
    if(aBoneIds[i] >= 0 && aBoneIds[i] < MAX_BONES){
      transform += uBones[aBoneIds[i]] * aWeights[i];
    }
  }

  if(transform == mat4(0.0)) transform = mat4(1.0);

  vec4 skinnedPos = transform * vec4(aPos, 1.0);
  
  gl_Position = projection * view * model * skinnedPos;
  Normal = aNormal;
}
