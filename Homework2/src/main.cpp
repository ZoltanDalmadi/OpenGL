#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <SOIL/SOIL.h>

#include <iostream>
#include <memory>
#include <vector>
#include <list>

#include "GLTargetCamera.h"
#include "GLPointLight.h"
#include "GLSpotLight.h"
#include "GLSphere.h"
#include "GLCube.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

glm::mat3 normalMatrix;
glm::mat4 projection;

GLTools::GLTargetCamera camera(glm::vec3(10.0f, 10.0f, 10.0f));

std::vector<GLTools::GLSpotLight> spotLights;

GLTools::GLPointLight pointLight;

glm::vec3 cubePos;

glm::vec3 ballPos(2.0f, -2.0f, -2.0f);
glm::vec3 ballDirection;
float ballSpeed = 0.1f;

bool keys[1024];

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void moveCube()
{
  if (keys[GLFW_KEY_W])
    cubePos -= glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_S])
    cubePos += glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_A])
    cubePos -= glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_D])
    cubePos += glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_Q])
    cubePos += glm::vec3(0.0f, 0.1f, 0.0f);

  if (keys[GLFW_KEY_E])
    cubePos -= glm::vec3(0.0f, 0.1f, 0.0f);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("shaders/vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("shaders/fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

GLuint loadTexture(const char *path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width, height;
  unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
  return textureID;
}

void setLightTargets(const glm::vec3& target)
{
  for (auto& light : spotLights)
    light.setTarget(target);
}

void setLightUniforms(const GLTools::GLShaderProgram& shaderProgram)
{
  for (const auto& light : spotLights)
    light.setShaderUniform(shaderProgram);
}

void wallCollision()
{
  // TODO
}

void cubeCollision()
{
  // TODO
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "HomeWork2", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, key_callback);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  spotLights.emplace_back(glm::vec3(10.0f, 10.0f, 10.0f));
  spotLights.back().setShaderName("spotLight[0]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(-10.0f, 10.0f, 10.0f));
  spotLights.back().setShaderName("spotLight[1]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(-10.0f, 10.0f, -10.0f));
  spotLights.back().setShaderName("spotLight[2]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(10.0f, 10.0f, -10.0f));
  spotLights.back().setShaderName("spotLight[3]");
  spotLights.back().m_energy.second = 10.0f;

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  ballDirection = glm::normalize(glm::ballRand(5.0f));
}

int main()
{
  init();

  GLTools::GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  GLTools::GLSphere ball(1.0f, 24, 24);
  ball.initialize();

  GLTools::GLCube room(24.0f, true);
  room.initialize();

  GLTools::GLCube cube(2.0f);
  cube.initialize();

  setLightTargets(cubePos);

  GLuint ballTexture = loadTexture("textures/ball.png");
  GLuint ballSpecularTexture = loadTexture("textures/ball_specular.png");

  GLuint wallTexture = loadTexture("textures/brick.jpg");
  GLuint wallSpecularTexture = loadTexture("textures/brick_specular.jpg");

  GLuint cubeTexture = loadTexture("textures/crate.jpg");
  GLuint cubeSpecularTexture = loadTexture("textures/crate_specular.jpg");

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.use();
    shaderProgram.setUniformValue("view", camera.m_viewMatrix);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("camPos", camera.m_position);

    shaderProgram.setUniformValue("material.diffuse", 0);
    shaderProgram.setUniformValue("material.specular", 1);
    shaderProgram.setUniformValue("material.shininess", 32.0f);

    pointLight.setShaderUniform(shaderProgram);
    setLightUniforms(shaderProgram);

    glm::mat4 roomModel;
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(roomModel)));
    shaderProgram.setUniformValue("model", roomModel);
    shaderProgram.setUniformValue("normalMatrix", normalMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, wallSpecularTexture);
    room.draw();

    glm::mat4 cubeModel;
    cubeModel = glm::translate(cubeModel, cubePos);
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
    shaderProgram.setUniformValue("model", cubeModel);
    shaderProgram.setUniformValue("normalMatrix", normalMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeSpecularTexture);
    cube.draw();

    glm::mat4 ballModel;
    ballModel = glm::translate(ballModel, ballPos);
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(ballModel)));
    shaderProgram.setUniformValue("model", ballModel);
    shaderProgram.setUniformValue("normalMatrix", normalMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ballTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ballSpecularTexture);
    ball.draw();

    glfwSwapBuffers(window);

    moveCube();
    setLightTargets(cubePos);

    ballPos += ballDirection * ballSpeed;
    camera.setTarget(ballPos);
  }

  glfwTerminate();

  return 0;
}
