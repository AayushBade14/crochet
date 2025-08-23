#version 460 core

layout(location=0)in vec3 aPos;

out VS_OUT{
  vec3 pos;
}vs_out;
  
void main(){
  gl_Position = vec4(aPos, 1.0);
  vs_out.pos = aPos;
}
