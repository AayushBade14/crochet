#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  UV = aUV;

  gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}
