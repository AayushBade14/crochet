#version 460 core

out vec4 fragColor;

in vec3 Normal;

void main(){
  fragColor = vec4(Normal, 1.0);
}
