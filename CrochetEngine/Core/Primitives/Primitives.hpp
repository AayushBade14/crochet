#pragma once

#include <vector>

namespace Crochet::Core {
  inline const std::vector<float> triangleVertices = {
    // position         // color      // texcoord   // normals
    -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,  0.0f,0.0f,-1.0f,
    0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f, 1.0f,0.0f,  0.0f,0.0f,-1.0f,
    0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f, 0.5f,1.0f,  0.0f,0.0f,-1.0f
  };

  inline const std::vector<float> quadVertices = {
    // position       // color        // texcoord // normals
    -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,  0.0f,0.0f,-1.0f,
    0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f, 1.0f,0.0f,  0.0f,0.0f,-1.0f,
    0.5f,0.5f,0.0f,   0.0f,0.0f,1.0f, 1.0f,1.0f,  0.0f,0.0f,-1.0f,

    0.5f,0.5f,0.0f,   0.0f,0.0f,1.0f, 1.0f,1.0f,  0.0f,0.0f,-1.0f,
    -0.5f,0.5f,0.0f,  0.5f,0.2f,0.1f, 0.0f,1.0f,  0.0f,0.0f,-1.0f,
    -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f, 0.0f,0.0f,  0.0f,0.0f,-1.0f
  };
}
