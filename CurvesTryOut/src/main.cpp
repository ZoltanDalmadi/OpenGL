#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "GLShaderProgram.h"

//A görbe kipróbálása, illettve tárgyak görbe mentén lévõ mozgatásának kipróbálására készült project

using namespace GLTools;

// CONSTANTS --------------------------------------------------------------
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;


// GLOBAL VARIABLES -------------------------------------------------------
GLFWwindow *window;

//---
GLuint vaoHandlers;
glm::mat4x3 G;
glm::mat4 N;
glm::mat4 M;
glm::vec3 Q;
glm::vec4 T;
glm::mat4x3 C;
GLdouble tParams[3] = { -1.0, 0.0, 1.0 };
std::vector<float> points;

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

  //glViewport(0, 0, WIDTH, HEIGHT);
  //glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void setupShaders(GLShaderProgram& shaderProgram)
{
  auto vertexShader = std::make_shared<GLShader>
                      (GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("shaders/vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader = std::make_shared<GLShader>
                        (GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("shaders/fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

//---
void createVBOs()
{
  G[0][2] = 1.0f;
  G[1][2] = 1.0f;
  G[2][2] = 1.0f;
  G[3][2] = 1.0f;

  G[0][0] = 0.0;
  G[0][1] = 0.0;

  G[1][0] = 0.3;
  G[1][1] = 0.7;

  G[2][0] = 0.8;
  G[2][1] = 0.8;

  G[3][0] = 0.8;
  G[3][1] = 0.8;

  N[0][0] = tParams[0] * tParams[0] * tParams[0];
  N[1][0] = tParams[0] * tParams[0];
  N[2][0] = tParams[0];
  N[3][0] = 1.0f;

  N[0][1] = tParams[1] * tParams[1] * tParams[1];
  N[1][1] = tParams[1] * tParams[1];
  N[2][1] = tParams[1];
  N[3][1] = 1.0f;

  N[0][2] = tParams[2] * tParams[2] * tParams[2];
  N[1][2] = tParams[2] * tParams[2];
  N[2][2] = tParams[2];
  N[3][2] = 1.0f;

  N[0][3] = 3 * tParams[0] * tParams[0];
  N[1][3] = 2 * tParams[0];
  N[2][3] = 1.0;
  N[3][3] = 0.0f;

  M = glm::inverse(N);

  C = G * M;

  for (GLdouble t = tParams[0]; t <= tParams[2];
       t += 0.1f)
  {
    T.x = t * t * t;
    T.y = t * t;
    T.z = t;
    T.w = 1.0;

    Q = C * T;
    std::cout << Q.x << " " << Q.y << " " << std::endl;
    points.push_back(Q.x);
    points.push_back(Q.y);
    points.push_back(0.0f);
  }

  //const int size = points.size();
  //float *position = &points[0];
  float position[60];
  std::copy(points.begin(), points.end(), position);
  std::cout << points.size();

  float colorData[] =
  {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f
  };

  GLuint vboHandlers[2];
  glGenBuffers(2, vboHandlers);
  GLuint positionBuffer = vboHandlers[0];
  GLuint colorBuffer = vboHandlers[1];

  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), position,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vaoHandlers);
  glBindVertexArray(vaoHandlers);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

int main()
{

  init();

  auto shaderProgram = std::make_unique<GLShaderProgram>();
  setupShaders(*shaderProgram);

  shaderProgram->use();


  createVBOs();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoHandlers);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
