#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "GLShaderProgram.h"
#include "GLFPSCamera.h"
#include "GLSphere.h"
#include "GLPlane.h"
#include "GLPointLight.h"
#include "GLCurves.h"
#include "GLCurvePath.h"
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H


//A görbe kipróbálása, illettve tárgyak görbe mentén lévõ mozgatásának kipróbálására készült project

struct Character
{
  GLuint     TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;       // Size of glyph
  glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
  GLuint     Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

using namespace GLTools;

// CONSTANTS --------------------------------------------------------------
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// GLOBAL VARIABLES -------------------------------------------------------
GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
//GLTools::GLPointLight pointLight(glm::vec3(0.0f, 5.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);

std::array<glm::vec3, 4> temp = { glm::vec3(0.0f, 0.01f, 0.0f),
                                  glm::vec3(10.0f, 0.01f, 30.0f),
                                  glm::vec3(17.0f, 0.01f, -34.0f),
                                  glm::vec3(40.0f, 0.01f, 40.0f)
                                };
GLTools::GLCurves curve(temp);
std::array<glm::vec3, 2> temp1 = { glm::vec3(50.0f, 0.01f, -50.0f),
                                   glm::vec3(100.0f, 0.01f, 100.0f)
                                 };
GLTools::GLCurves curve1(curve, temp1);

GLTools::GLCurvePath path;
//std::unique_ptr<GLTools::GLSphere> targetSphere;
std::unique_ptr<GLTools::GLPlane> floorPlane;
glm::mat4 projection;

bool keys[1024];

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void moveCamera()
{
  if (keys[GLFW_KEY_W])
    camera.move(GLTools::GLFPSCamera::Direction::FORWARD);

  if (keys[GLFW_KEY_S])
    camera.move(GLTools::GLFPSCamera::Direction::BACKWARD);

  if (keys[GLFW_KEY_A])
    camera.move(GLTools::GLFPSCamera::Direction::LEFT);

  if (keys[GLFW_KEY_D])
    camera.move(GLTools::GLFPSCamera::Direction::RIGHT);

  if (keys[GLFW_KEY_SPACE])
    camera.move(GLTools::GLFPSCamera::Direction::UP);

  if (keys[GLFW_KEY_LEFT_CONTROL])
    camera.move(GLTools::GLFPSCamera::Direction::DOWN);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  auto xoffset = xpos - lastX;
  auto yoffset = ypos - lastY;
  lastX = xpos;
  lastY = ypos;
  camera.rotate(static_cast<float>(xoffset), static_cast<float>(yoffset));
}


void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Curves", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_CULL_FACE);
  glLineWidth(5);
  glPointSize(10);
  glClearColor(0.0f, 0.3f, 0.6f, 1.0f);
  curve.initialize();
  curve1.initialize();
  path.m_data.push_back(curve);
  path.m_data.push_back(curve1);
}

void setupShaders(GLShaderProgram& shaderProgram)
{
  auto vertexShader = std::make_shared<GLShader>
                      (GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("shaders/curves_vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader = std::make_shared<GLShader>
                        (GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("shaders/curves_fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  auto geometryShader = std::make_shared<GLShader>
                        (GLShader::shaderType::GEOMETRY_SHADER);
  geometryShader->loadSource("shaders/curves_geometry_shader.glsl");
  geometryShader->compile();
  std::cout << geometryShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.addShader(geometryShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}


void setupShaders1(GLShaderProgram& shaderProgram)
{
  auto vertexShader = std::make_shared<GLShader>
                      (GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("shaders/vertex_shader1.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader = std::make_shared<GLShader>
                        (GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("shaders/fragment_shader1.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

void setupShaders2(GLShaderProgram& shaderProgram)
{
  auto vertexShader = std::make_shared<GLShader>
                      (GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("shaders/text_vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader = std::make_shared<GLShader>
                        (GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("shaders/text_fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

GLuint VAO, VBO;
void RenderText(GLShaderProgram& s, std::string text, GLfloat x,
                GLfloat y,
                GLfloat scale, glm::vec3 color)
{
  // Activate corresponding render state
  s.use();
  s.setUniformValue("textColor", color.x, color.y,
                    color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  // Iterate through all characters
  std::string::const_iterator c;

  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] =
    {
      { xpos, ypos + h, 0.0, 0.0 },
      { xpos, ypos, 0.0, 1.0 },
      { xpos + w, ypos, 1.0, 1.0 },

      { xpos, ypos + h, 0.0, 0.0 },
      { xpos + w, ypos, 1.0, 1.0 },
      { xpos + w, ypos + h, 1.0, 0.0 }
    };
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) *
         scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

int main()
{
  init();

  // szöveg rendereléshez kell

  FT_Library ft;

  if (FT_Init_FreeType(&ft))
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

  FT_Face face;

  if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);

  if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character =
    {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  // Destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  auto shaderProgram2 = std::make_unique<GLShaderProgram>();
  setupShaders2(*shaderProgram2);

  glm::mat4 projection2 = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //eddig szöveghez kellõ cuccok
  auto defaultMaterial = std::make_unique<GLTools::GLMaterial>();

  floorPlane = std::make_unique<GLPlane>(100.0f, 100.0f);
  floorPlane->initialize();
  floorPlane->m_material = defaultMaterial.get();

  auto shaderProgram1 = std::make_unique<GLShaderProgram>();
  setupShaders1(*shaderProgram1);

  auto shaderProgram = std::make_unique<GLShaderProgram>();
  setupShaders(*shaderProgram);

  projection = glm::perspective(glm::radians(50.0f), (float)WIDTH / HEIGHT, 0.01f,
                                500.0f);

  float t = 0.0f;


  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    moveCamera();
    shaderProgram1->use();
    auto model = rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f,
                        0.0f, 0.0f));
    shaderProgram1->setUniformValue("MVP",
                                    projection * camera.m_viewMatrix * model);

    floorPlane->draw();

    shaderProgram->use();

    shaderProgram->setUniformValue("MVP",
                                   projection * camera.m_viewMatrix);


    path.draw();
    shaderProgram2->use();
    shaderProgram2->setUniformValue("projection",
                                    projection);
    RenderText(*shaderProgram2, "This is sample text", 25.0f, 25.0f, 1.0f,
               glm::vec3(0.5,
                         0.8f, 0.2f));
    RenderText(*shaderProgram2, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f,
               glm::vec3(0.3,
                         0.7f, 0.9f));
    //std::pair<glm::vec3, glm::vec3> asd = path.getPositionAndTangent(t);
    /*std::cout << t << ": " << asd.first.x << " " << asd.first.y << " " <<
              asd.first.z << " :: " << asd.second.x << " " << asd.second.y << " " <<
              asd.second.z << "\n";*/
    t += 0.001;

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
