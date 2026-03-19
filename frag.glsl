#version 460 core

out vec4 fragColor;

in vec2 UV;

uniform sampler2D tex;

void main()
{
  fragColor = texture(tex, UV);
}
