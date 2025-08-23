#version 460 core

out vec4 fragColor;

void main(){
  vec2 st = gl_FragCoord.xy;
  float x = st.x;
  vec3 color = x * vec3(0.9,0.4,0.2);

  fragColor = vec4(color, 1.0);
}
