#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/intersect.hpp>

#include <SOIL/SOIL.h>

#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <list>

#include "GLTargetCamera.h"
#include "GLPointLight.h"
#include "GLSpotLight.h"
#include "GLSphere.h"
#include "GLCube.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

const float ROOMSIZE = 24.0f;
const float CUBESIZE = 5.0f;
const float BALLRADIUS = 1.0f;

GLFWwindow *window;

glm::mat3 normalMatrix;
glm::mat4 projection;

GLTools::GLTargetCamera camera(glm::vec3(10.0f, 10.0f, 10.0f));

std::vector<GLTools::GLSpotLight> spotLights;

GLTools::GLPointLight pointLight;

glm::vec3 cubePos;

struct CollisionPlane
{
  glm::vec3 origin;
  glm::vec3 normal;
};

std::vector<CollisionPlane> walls;
std::vector<CollisionPlane> cubeFaces;

glm::vec3 ballPos(5.0f, -5.0f, -5.0f);
glm::vec3 ballDirection;
float ballSpeed = 0.2f;

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
  glm::vec3 delta(0.0f);

  if (keys[GLFW_KEY_W])
    delta -= glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_S])
    delta += glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_A])
    delta -= glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_D])
    delta += glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_Q])
    delta += glm::vec3(0.0f, 0.1f, 0.0f);

  if (keys[GLFW_KEY_E])
    delta -= glm::vec3(0.0f, 0.1f, 0.0f);

  cubePos += delta;

  for (auto& cubeFace : cubeFaces)
    cubeFace.origin += delta;
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
  float distance;

  for (const auto& wall : walls)
  {
    distance = glm::dot(ballPos - wall.origin, wall.normal);

    if (distance < BALLRADIUS)
      ballDirection = glm::normalize(glm::reflect(ballDirection, wall.normal));
  }
}

bool isOutSidePlane(const glm::vec3 point, const CollisionPlane& plane)
{
  return glm::dot(point - plane.origin, plane.normal) > BALLRADIUS;
}

bool ballIntersectsPlane(const glm::vec3 point, const CollisionPlane& plane)
{
  return glm::abs(glm::dot(point - plane.origin, plane.normal)) <= BALLRADIUS;
}

void cubeCollision()
{
  auto in_front = !isOutSidePlane(ballPos, cubeFaces[0]);
  auto in_right = !isOutSidePlane(ballPos, cubeFaces[1]);
  auto in_back = !isOutSidePlane(ballPos, cubeFaces[2]);
  auto in_left = !isOutSidePlane(ballPos, cubeFaces[3]);
  auto in_bottom = !isOutSidePlane(ballPos, cubeFaces[4]);
  auto in_top = !isOutSidePlane(ballPos, cubeFaces[5]);

  if (ballIntersectsPlane(ballPos, cubeFaces[0]) &&
      in_top && in_bottom && in_left && in_right)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[0].normal));
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[1]) &&
      in_top && in_bottom && in_front && in_back)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[1].normal));
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[2]) &&
      in_top && in_bottom && in_left && in_right)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[2].normal));
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[3]) &&
      in_top && in_bottom && in_front && in_back)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[3].normal));
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[4]) &&
      in_left && in_right && in_front && in_back)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[4].normal));
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[5]) &&
      in_left && in_right && in_front && in_back)
  {
    ballDirection =
      glm::normalize(glm::reflect(ballDirection, cubeFaces[5].normal));
  }
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

  spotLights.emplace_back(glm::vec3(12.0f, 12.0f, 12.0f));
  spotLights.back().setShaderName("spotLight[0]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(-12.0f, 12.0f, 12.0f));
  spotLights.back().setShaderName("spotLight[1]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(-12.0f, 12.0f, -12.0f));
  spotLights.back().setShaderName("spotLight[2]");
  spotLights.back().m_energy.second = 10.0f;

  spotLights.emplace_back(glm::vec3(12.0f, 12.0f, -12.0f));
  spotLights.back().setShaderName("spotLight[3]");
  spotLights.back().m_energy.second = 10.0f;

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-2, 2);

  ballDirection = glm::normalize(glm::vec3(dis(gen), dis(gen), dis(gen)));

  glPointSize(20.0f);
}

int main()
{
  init();

  GLTools::GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  GLTools::GLSphere ball(BALLRADIUS, 24, 24);
  ball.initialize();

  GLTools::GLCube room(ROOMSIZE, true);
  room.initialize();

  GLTools::GLCube cube(CUBESIZE);
  cube.initialize();

  CollisionPlane cp;

  for (size_t i = 0; i < 24; i += 4)
  {
    cp.origin = room.m_vertices[i].position;
    cp.normal = room.m_vertices[i].normal;
    walls.push_back(cp);
  }

  for (size_t i = 0; i < 24; i += 4)
  {
    cp.origin = cube.m_vertices[i].position;
    cp.normal = cube.m_vertices[i].normal;
    cubeFaces.push_back(cp);
  }

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
    wallCollision();
    cubeCollision();
    ballPos += ballDirection * ballSpeed;
    camera.setTarget(ballPos);
  }

  glfwTerminate();

  return 0;
}
