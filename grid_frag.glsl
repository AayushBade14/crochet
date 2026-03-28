#version 460 core
out vec4 fragColor;
in vec2 fragPos;

uniform mat4 invProj;

uniform float TILE_SIZE;
uniform float CHUNK_SIZE;

uniform float x_min;
uniform float x_max;
uniform float y_min;
uniform float y_max;

uniform int tool;

void main()
{
    vec4 world = invProj * vec4(fragPos, 0.0, 1.0);
    float worldX = world.x / world.w;
    float worldY = world.y / world.w;

    float fwX = fwidth(worldX);
    float fwY = fwidth(worldY);

    float thinX = mod(worldX, TILE_SIZE);
    float thinY = mod(worldY, TILE_SIZE);
    float thinThickness = max(1.0, max(fwX, fwY));

    float thickX = mod(worldX, CHUNK_SIZE * TILE_SIZE);
    float thickY = mod(worldY, CHUNK_SIZE * TILE_SIZE);
    float thickThickness = max(3.0, max(fwX, fwY) * 1.5);

    bool isThin  = thinX  < thinThickness  || thinY  < thinThickness;
    bool isThick = thickX < thickThickness || thickY < thickThickness;
  
    if(tool == 1)
    {
      bool isBound = (worldX > x_min) && (worldX < x_max) && (worldY > y_min) && (worldY < y_max);
      if(isBound){
        if(isThick)
          fragColor = vec4(0.0, 0.8, 0.0, 1.0);
        if(isThin)
          fragColor = vec4(0.0, 0.2, 0.0, 1.0);
        else
          fragColor = vec4(0.0, 0.2, 0.0, 0.4);
      }
      else{
        if(isThick)
          fragColor = vec4(0.8, 0.8, 0.8, 1.0);
        else if(isThin)
          fragColor = vec4(0.2, 0.2, 0.2, 1.0);
        else
          discard;
      }
    }
    else{
      if(isThick)
        fragColor = vec4(0.8, 0.8, 0.8, 1.0);
      else if(isThin)
        fragColor = vec4(0.2, 0.2, 0.2, 1.0);
      else
        discard;
    }
  }
