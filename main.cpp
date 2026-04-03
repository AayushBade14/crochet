#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <ctime>

#include "./Vendor/stb_image/stb_image.h"

#include "./World.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int Width = 1920;
int Height = 1013;
std::string Title = "crochet-editor";

glm::vec2 camPos = glm::vec2(1.0f);

float dt = 0.0f;
float lastFrame = 0.0f;

float zoom = 1.0f;

GLFWwindow* window = nullptr;

glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);

double xMouseScreen = 0.0f;
double yMouseScreen = 0.0f;

float xMouseWorld = 0.0f;
float yMouseWorld = 0.0f;

int brush_size = 1;
unsigned int tex;
int selectedTile = 1;

struct Shader
{
  unsigned int m_ID;

  Shader(const std::string& vertPath, const std::string& fragPath)
  {
    std::string vCode = LoadFile(vertPath);
    std::string fCode = LoadFile(fragPath);
    unsigned int vert = CompileShader(vCode, true);
    unsigned int frag = CompileShader(fCode, false);

    CreateShaderProgram(vert, frag);
  }

  ~Shader()
  {
    glDeleteProgram(m_ID);
  }

  void Use()
  {
    glUseProgram(m_ID);
  }

  template <typename T>
  void SetValue(const std::string& name, const T& val)
  {
    const unsigned int loc = glGetUniformLocation(m_ID, name.c_str());

    if constexpr(std::is_same_v<T, int>) glUniform1i(loc, val);
    else if constexpr(std::is_same_v<T, bool>) glUniform1i(loc, (int)val);
    else if constexpr(std::is_same_v<T, float>) glUniform1f(loc, val);
    else if constexpr(std::is_same_v<T, glm::vec2>) glUniform2fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T, glm::vec3>) glUniform3fv(loc, 1, glm::value_ptr(val));
    else if constexpr(std::is_same_v<T, glm::mat4>) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  }

private:

  std::string LoadFile(const std::string& path)
  {
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try
    {
      file.open(path);
      std::stringstream stream;
      stream << file.rdbuf();
      code = stream.str();
      file.close();
    }
    catch(const std::ifstream::failure& e)
    {
      std::cerr << e.what() << std::endl;
      exit(1);
    }

    return code;
  }

  unsigned int CompileShader(const std::string& src, bool isVert)
  {
    const char* code = src.c_str();
    int success;
    char infoLog[512];
    unsigned int shader = isVert? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cerr << infoLog << std::endl;
    }

    return shader;
  }

  void CreateShaderProgram(unsigned int vert, unsigned int frag)
  {
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vert);
    glAttachShader(m_ID, frag);
    glLinkProgram(m_ID);

    int success;
    char infoLog[512];

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
      std::cerr << infoLog << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
  }
};

void ProcessInput()
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
    camPos.y += 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
    camPos.y -= 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
    camPos.x -= 4 * TILE_SIZE * dt;
  if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
    camPos.x += 4 * TILE_SIZE * dt;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  zoom -= (float)yoffset * dt * 5.0f;
  if(zoom <= 0.0f)
  {
    zoom = 0.1f;
  }
}


void ScreenToWorld()
{
  float xNDC = (2.0f * xMouseScreen)/(float)Width - 1.0f;
  float yNDC = 1.0f - (2.0f * yMouseScreen)/(float)Height;

  glm::vec4 ray_clip = glm::vec4(xNDC, yNDC, 0.0f, 1.0f);

  glm::vec4 ray_eye = glm::inverse(projection) * ray_clip; 
  ray_eye /= ray_eye.w;

  glm::vec3 ray_world = glm::inverse(view) * ray_eye;

  xMouseWorld = ray_world.x;
  yMouseWorld = ray_world.y;
}

unsigned int paintToolTex, chunkToolTex;

void InitUI()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  glGenTextures(1, &paintToolTex);
  glGenTextures(1, &chunkToolTex);

  int width, height, nrChannels;
  GLenum flag = GL_RGB;
  unsigned char* data = nullptr;

  glBindTexture(GL_TEXTURE_2D, paintToolTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load("../ToolIcons/tile_paint_tool.png", &width, &height, &nrChannels, 0);
  flag = nrChannels == 3? GL_RGB : (nrChannels == 4? GL_RGBA : GL_RED);
  if(data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
  }
  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, chunkToolTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load("../ToolIcons/chunk_paint_tool.png", &width, &height, &nrChannels, 0);
  flag = nrChannels == 3? GL_RGB : (nrChannels == 4? GL_RGBA : GL_RED);
  if(data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, flag, width, height, 0, flag, GL_UNSIGNED_BYTE, data);
  }
  stbi_image_free(data);
  
  glBindTexture(GL_TEXTURE_2D, 0);
}

void CleanUI()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void BeginUIFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void EndUIFrame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

enum Tool { TOOL_PAINT, TOOL_CHUNK };
Tool activeTool = TOOL_PAINT; // your current tool state

void DrawToolbar()
{
  ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
    ImVec4 activeColor  = ImVec4(0.2f, 0.8f, 0.2f, 1.0f); // green tint when selected
    ImVec4 defaultColor = ImGui::GetStyleColorVec4(ImGuiCol_Button); // normal button color

    // Paint tool
    ImGui::PushStyleColor(ImGuiCol_Button, activeTool == TOOL_PAINT ? activeColor : defaultColor);
    if(ImGui::ImageButton("paint_tool", (ImTextureID)(intptr_t)paintToolTex, ImVec2(32, 32)))
        activeTool = TOOL_PAINT;
    ImGui::PopStyleColor();

    // Chunk tool
    ImGui::PushStyleColor(ImGuiCol_Button, activeTool == TOOL_CHUNK ? activeColor : defaultColor);
    if(ImGui::ImageButton("chunk_tool", (ImTextureID)(intptr_t)chunkToolTex, ImVec2(32, 32)))
        activeTool = TOOL_CHUNK;
    ImGui::PopStyleColor();

  ImGui::End();
}

void DrawProfiler()
{
  static float fps[100] = {};
  static int offset = 0;

  ImGui::Begin("PROFILING");

  float currentFPS = ImGui::GetIO().Framerate;
  fps[offset] = currentFPS;
  offset = (offset + 1) % 100;


  ImGui::Text("FPS: %.1f", currentFPS);
  
  ImGui::Text("FRAME-TIME: %.3f", 1000.0f/currentFPS);
  ImGui::PlotLines(
    "FPS",
    fps,
    100,
    0,
    NULL,
    0.0f,
    200.0f,
    ImVec2(0, 80)
  );
  
  float min = fps[0];
  float max = fps[0];
  float sum = 0;

  for(int i = 0; i < 100; i++)
  {
    min = std::min(min, fps[i]);
    max = std::max(max, fps[i]);
    sum += fps[i];
  }

  float avg = sum/100;

  ImGui::Text("AVG-FPS: %.1f", avg);
  ImGui::Text("MIN-FPS: %.1f", min);
  ImGui::Text("MAX-FPS: %.1f", max);
  
  ImGui::Separator();

  ImGui::Text("TOTAL-CHUNKS: %d", WORLD_SIZE * WORLD_SIZE);
  ImGui::Text("TOTAL-TILES: %d", WORLD_SIZE * WORLD_SIZE * CHUNK_SIZE * CHUNK_SIZE);

  ImGui::End(); 
}

void DrawLayerMenu()
{
  ImGui::Begin("LAYER-MENU");

  ImGui::End();
}

void DrawBrushProps()
{
  ImGui::Begin("BRUSH-PROPERTIES");
  
  ImGui::SliderInt("BRUSH-SIZE", &brush_size, 1, 32);

  ImGui::End();
}

void DrawTilePalette()
{
    ImGui::Begin("Tile Palette");
    ImGui::BeginChild("TileScroll", ImVec2(0, 300), true);

    int atlasRows = ATLAS_SIZE / TILE_SIZE;
    int atlasCols = ATLAS_SIZE / TILE_SIZE;

    for (int y = 0; y < atlasRows; y++)
    {
        for (int x = 0; x < atlasCols; x++)
        {
            int tileID = y * atlasCols + x;

            float u0 = (x * TILE_SIZE) / (float)ATLAS_SIZE;
            float v0 = (y * TILE_SIZE) / (float)ATLAS_SIZE;
            float u1 = ((x + 1) * TILE_SIZE) / (float)ATLAS_SIZE;
            float v1 = ((y + 1) * TILE_SIZE) / (float)ATLAS_SIZE;

            ImGui::PushID(tileID + 1);  // +1 to avoid ID 0
            if (ImGui::ImageButton(
                "",
                (ImTextureID)(intptr_t)tex,
                ImVec2(32, 32),
                ImVec2(u0, v1),
                ImVec2(u1, v0)))
            {
                selectedTile = tileID;  // still store the real 0-based ID
            }
            ImGui::PopID();

            if (x < atlasCols - 1)
                ImGui::SameLine();
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void DrawUI()
{
  BeginUIFrame();
  
  DrawProfiler(); 
  DrawToolbar();
  DrawLayerMenu();
  
  if(activeTool == Tool::TOOL_PAINT){
    DrawBrushProps();
    DrawTilePalette();
  }

  EndUIFrame();
}


float gridVertices[] = {
  -1.0f, -1.0f, 
   1.0f, -1.0f,
   1.0f,  1.0f,

   1.0f,  1.0f,
  -1.0f,  1.0f,
  -1.0f, -1.0f
};

int main(void)
{ 
  
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(Width,
                            Height,
                            Title.c_str(),
                            nullptr,
                            nullptr);

  glfwMakeContextCurrent(window);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  srand(time(0));

  stbi_set_flip_vertically_on_load(true);
  
  Shader shader("../vert.glsl", "../frag.glsl");
  Shader b_shader("../brush_vert.glsl", "../brush_frag.glsl");
  Shader g_shader("../grid_vert.glsl", "../grid_frag.glsl");
  unsigned int grid_vao;
  unsigned int grid_vbo;
  glGenVertexArrays(1, &grid_vao);
  glGenBuffers(1, &grid_vbo);
  glBindVertexArray(grid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
  glBindVertexArray(0);

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char* data  = stbi_load("../test.png", &width, &height, &nrChannels, 0);
  if(data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);
  
  shader.Use();
  shader.SetValue("tex", 0);
  
  World m_World;

  m_World.Init();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  
  InitUI();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  while(!glfwWindowShouldClose(window))
  {
    float currentFrame = (float)glfwGetTime();
    dt = currentFrame -  lastFrame;
    lastFrame = currentFrame;
    
    //std::cout << "FrameRate: " << 1.0f/dt << std::endl;

    glfwPollEvents();
    
    glfwGetCursorPos(window, &xMouseScreen, &yMouseScreen);
    ScreenToWorld();
 
    ProcessInput();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    float aspect = (float)Width/(float)Height;

    glm::mat4 model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-camPos.x, -camPos.y, 0.0f));
    projection = glm::ortho(0.0f, (float)16*TILE_SIZE * aspect * zoom, 0.0f, (float)16*TILE_SIZE * zoom);
    
    glm::mat4 invProj = glm::mat4(1.0f);
    invProj = glm::inverse(projection * view);

    shader.Use();
    shader.SetValue("model", model);
    shader.SetValue("view", view);
    shader.SetValue("projection", projection);

    m_World.Render(window, xMouseWorld, yMouseWorld, camPos.x, camPos.y, zoom, aspect, !ImGui::GetIO().WantCaptureMouse);
    
    g_shader.Use();
    g_shader.SetValue("invProj", invProj);
    g_shader.SetValue("TILE_SIZE", (float)TILE_SIZE);
    g_shader.SetValue("CHUNK_SIZE", (float)CHUNK_SIZE);
    
    g_shader.SetValue("tool", (int)(activeTool == Tool::TOOL_CHUNK));

    if(activeTool == Tool::TOOL_CHUNK)
    {
        float chunkWorld = (float)(CHUNK_SIZE * TILE_SIZE);
        // snap to chunk grid instead of centering on mouse
        float snappedX = floor(xMouseWorld / chunkWorld) * chunkWorld;
        float snappedY = floor(yMouseWorld / chunkWorld) * chunkWorld;

        g_shader.SetValue("x_min", snappedX);
        g_shader.SetValue("x_max", snappedX + chunkWorld);
        g_shader.SetValue("y_min", snappedY);
        g_shader.SetValue("y_max", snappedY + chunkWorld);
        
        bool mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        static bool wasPressed = false;
        if(mouseState == GLFW_PRESS && !wasPressed && !ImGui::GetIO().WantCaptureMouse)
        {
          m_World.AddChunk(snappedX, snappedY);
          wasPressed = true;
        }

        if(mouseState == GLFW_RELEASE)
        {
          wasPressed = false;
        }

        bool mouseStateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        static bool  wasPressedR = false;

        if(mouseStateR == GLFW_PRESS && !wasPressedR && !ImGui::GetIO().WantCaptureMouse)
        {
          m_World.EraseChunk(snappedX, snappedY);
          wasPressedR = true;
        }

        if(mouseStateR == GLFW_RELEASE)
        {
          wasPressedR = false;
        }
        
    }
    else if(activeTool == Tool::TOOL_PAINT)
    {
      glm::mat4 brush_model = glm::mat4(1.0f);
      brush_model = glm::translate(brush_model, glm::vec3(xMouseWorld, yMouseWorld, 0.0));
      brush_model = glm::scale(brush_model, glm::vec3(brush_size * TILE_SIZE/ 2.0f));
      
      b_shader.Use();
      b_shader.SetValue("model", brush_model);
      b_shader.SetValue("view", view);
      b_shader.SetValue("projection", projection);

      glBindVertexArray(grid_vao);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
      
      bool mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      if(mouseState == GLFW_PRESS)
      {
        if(!ImGui::GetIO().WantCaptureMouse)
        {
          float offset = brush_size * TILE_SIZE/2.0f;
          
          float snappedX = floor(xMouseWorld / TILE_SIZE) * TILE_SIZE + TILE_SIZE * 0.5f;
          float snappedY = floor(yMouseWorld / TILE_SIZE) * TILE_SIZE + TILE_SIZE * 0.5f;
          
          float left = snappedX - offset;
          float right = snappedX + offset;
          float bottom = snappedY - offset;
          float top = snappedY + offset;

          int x1 = (int)floor(left / (CHUNK_SIZE * TILE_SIZE));
          int x2 = (int)floor(right / (CHUNK_SIZE * TILE_SIZE));
          int y1 = (int)floor(bottom / (CHUNK_SIZE * TILE_SIZE));
          int y2 = (int)floor(top / (CHUNK_SIZE * TILE_SIZE));

          for(int y = y1; y <= y2; y++)
          {
            for(int x = x1; x <= x2; x++)
            {
              int m_X = x * CHUNK_SIZE * TILE_SIZE;
              int m_Y = y * CHUNK_SIZE * TILE_SIZE;

              auto it = m_World.m_Chunks.find({m_X, m_Y});
              if(it != m_World.m_Chunks.end())
              {
                Chunk& chunk = it->second;

                float l = left - chunk.m_ChunkCoords.m_X;
                float r = right - chunk.m_ChunkCoords.m_X;
                float b = bottom - chunk.m_ChunkCoords.m_Y;
                float t = top - chunk.m_ChunkCoords.m_Y;

                float x_start = std::max(0.0f, l);
                float x_end = std::min(r, (float)CHUNK_SIZE * TILE_SIZE);
                float y_start = std::max(0.0f, b);
                float y_end = std::min(t, (float)CHUNK_SIZE * TILE_SIZE);
                
                int x1_ = x_start/TILE_SIZE;
                int x2_ = x_end/TILE_SIZE;

                int y1_ = y_start/TILE_SIZE;
                int y2_ = y_end/TILE_SIZE;

                for(int i = y1_; i < y2_; i++)
                {
                  for(int j = x1_; j < x2_; j++)
                  {
                    chunk.m_Tiles[j][i].m_ID = selectedTile;
                    chunk.ReBakeVertices(i, j);
                  }
                }
              }
            }
          }
        }
      }
    }
    
    g_shader.Use();
    glBindVertexArray(grid_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    DrawUI();

    glfwSwapBuffers(window);
  }
  
  CleanUI();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
