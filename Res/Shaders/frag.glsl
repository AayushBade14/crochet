#version 460 core

out vec4 fragColor;

void main(){
  vec2 st = gl_FragCoord.xy/vec2(1920.0,1013.0);
  float x = st.x;
  float y = st.y;
  vec3 color = x * y * vec3(0.9,0.4,0.2);

  fragColor = vec4(color, 1.0);
}
