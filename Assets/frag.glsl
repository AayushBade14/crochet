#version 460 core

out vec4 fragColor;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D tex;

void main(){
  fragColor = texture(tex, TexCoord);
}
