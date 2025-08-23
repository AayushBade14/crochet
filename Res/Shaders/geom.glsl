#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in VS_OUT{
  vec3 pos;
}gs_in[];

out GS_OUT{
  vec3 pos;
}gs_out;

void main(){
  gl_Position = vec4(gs_in[0].pos,1.0);
  EmitVertex();

  gl_Position = vec4(gs_in[1].pos,1.0);
  EmitVertex();

  gl_Position = vec4(gs_in[2].pos,1.0) + vec4(-0.5,0.0,0.0,0.0);
  EmitVertex();

  gl_Position = vec4(gs_in[1].pos,1.0) + vec4(0.0,1.0,0.0,0.0);
  EmitVertex();

  EndPrimitive();
}
