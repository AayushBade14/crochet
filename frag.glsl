#version 460 core

out vec4 fragColor;

in vec2 UV;

uniform sampler2D tex;

void main()
{
  if(UV.x >= 0.0 && UV.y >= 0.0)
    fragColor = texture(tex, UV);
  else
    fragColor = vec4(0.0);
}
