#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "GLFPSCamera.h"
#include "GLModel.h"
#include "GLPointLight.h"
#include "Tower.h"
#include "GLSphere.h"

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, -5.0f));
GLTools::GLPointLight pointLight(glm::vec3(0.0f, 5.0f, 0.0f));

glm::vec3 target(5.0f, 5.0f, 5.0f);

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::unique_ptr<Tower> tower;
std::unique_ptr<GLTools::GLSphere> targetSphere;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_C && action == GLFW_PRESS)
    tower->clearTarget();

  if (key == GLFW_KEY_V && action == GLFW_PRESS)
    tower->setTarget(&target);
}

void moveCamera()
{
  if (keys[GLFW_KEY_W])
    camera.move(GLTools::GLFPSCamera::Direction::FORWARD_FLOOR);

  if (keys[GLFW_KEY_S])
    camera.move(GLTools::GLFPSCamera::Direction::BACKWARD_FLOOR);

  if (keys[GLFW_KEY_A])
    camera.move(GLTools::GLFPSCamera::Direction::LEFT);

  if (keys[GLFW_KEY_D])
    camera.move(GLTools::GLFPSCamera::Direction::RIGHT);
}

void moveTarget()
{
  if (keys[GLFW_KEY_UP])
    target += glm::vec3(0.0f, 0.2f, 0.0f);

  if (keys[GLFW_KEY_DOWN])
    target -= glm::vec3(0.0f, 0.2f, 0.0f);

  if (keys[GLFW_KEY_LEFT])
    target += glm::vec3(0.2f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_RIGHT])
    target -= glm::vec3(0.2f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_PAGE_UP])
    target += glm::vec3(0.0f, 0.0f, 0.2f);

  if (keys[GLFW_KEY_PAGE_DOWN])
    target -= glm::vec3(0.0f, 0.0f, 0.2f);
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

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/vertex_shader.glsl");
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/fragment_shader.glsl");
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "TowerDefense", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.3f, 0.6f, 1.0f);
}

void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{
  glm::mat4 modelModel;
  modelModel = translate(modelModel, target);
  auto normalMatrix = glm::mat3(transpose(inverse(modelModel)));
  shaderProgram.setUniformValue("model", modelModel);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  targetSphere->draw();

  tower->update();
  tower->draw(shaderProgram);
}

int main()
{
  init();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram);

  shaderProgram->use();
  shaderProgram->setUniformValue("shadowMap", 0);
  shaderProgram->setUniformValue("projectTex", 3);
  shaderProgram->setUniformValue("rect", true);

  auto model = std::make_unique<GLTools::GLModel>("models/turret_base.obj");

  targetSphere = std::make_unique<GLTools::GLSphere>(1.0f, 24, 24);
  targetSphere->initialize();
  //targetSphere->m_material->m_diffuse = glm::vec3(1.0f);
  //targetSphere->m_material->m_specular = glm::vec3(1.0f);

  tower = std::make_unique<Tower>(model.get());
  tower->setTarget(&target);

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    moveCamera();
    moveTarget();

    shaderProgram->setUniformValue("viewProjection",
                                   projection * camera.m_viewMatrix);
    //shaderProgram->setUniformValue("camPos", camera.m_position);
    pointLight.setShaderUniform(*shaderProgram);

    renderScene(*shaderProgram);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
