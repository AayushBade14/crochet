#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aOffset;

//out VS_OUT{
  //vec3 pos;
//}vs_out;
  
void main(){
  gl_Position = vec4(aPos+aOffset, 1.0);
  //vs_out.pos = aPos;
}
