#pragma once

#include <API/OpenGL/BackEnd/GL_BackEnd.h>
#include <BackEnd/GLFW_BackEnd.h>

#include <API/OpenGL/Types/GL_Shader.h>
#include <API/OpenGL/Types/GL_VBO.h>
#include <API/OpenGL/Types/GL_VAO.h>

float vertices = {
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f,  0.0f,
  0.0f, 0.5f, 0.0f
};

void Render();
