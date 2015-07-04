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

/*Text*/
#include "GLText.h"

//A görbe kipróbálása, illettve tárgyak görbe mentén lévõ mozgatásának kipróbálására készült project
GLText text("fonts/arial.ttf");

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

int main()
{
  init();
  text.initialize();
  auto shaderProgram2 = std::make_unique<GLShaderProgram>();
  setupShaders2(*shaderProgram2);

  glm::mat4 projection2 = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

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
                                    projection2);
    //text.color = glm::vec3(0.5, 0.8f, 0.2f);
    text.draw(*shaderProgram2, "T - next tower", 25.0f, 25.0f, 0.2f);

    t += 0.001;

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
