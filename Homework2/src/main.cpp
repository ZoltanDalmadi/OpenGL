#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <random>
#include <vector>

#include "GLTargetCamera.h"
#include "GLPointLight.h"
#include "GLSpotLight.h"
#include "GLSphere.h"
#include "GLCube.h"
#include "GLTexture.h"

// CONSTANTS ------------------------------------------------------------------
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

const float ROOMSIZE = 24.0f;
const float CUBESIZE = 5.0f;
const float BALLRADIUS = 1.0f;
const float BALLSPEED = 0.4f;
const glm::vec3 CAMERAPOS = glm::vec3(10.0f, 10.0f, 10.0f);

// This struct holds the neccessary information for calculating collisions
struct CollisionPlane
{
  glm::vec3 origin;
  glm::vec3 normal;
};

GLFWwindow *window;

GLTools::GLTargetCamera camera(CAMERAPOS);

// Lights container
std::vector<GLTools::GLSpotLight> spotLights;
GLTools::GLPointLight pointLight;

// Vectors
glm::vec3 cubePos;
glm::vec3 ballPos(5.0f, -5.0f, -5.0f);
glm::vec3 ballDirection;
glm::vec3 ballRotationAxis;
float ballRotationAngle = 0.0f;

std::vector<CollisionPlane> walls;
std::vector<CollisionPlane> cubeFaces;

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
  bool moved = false;
  glm::vec3 delta(0.0f);

  if (keys[GLFW_KEY_W])
  {
    if (cubePos.z - (CUBESIZE / 2.0f) >= -ROOMSIZE / 2.0f)
    {
      delta -= glm::vec3(0.0f, 0.0f, 0.1f);
      moved = true;
    }
  }

  if (keys[GLFW_KEY_S])
  {
    if (cubePos.z + (CUBESIZE / 2.0f) <= ROOMSIZE / 2.0f)
    {
      delta += glm::vec3(0.0f, 0.0f, 0.1f);
      moved = true;
    }
  }

  if (keys[GLFW_KEY_A])
  {
    if (cubePos.x - (CUBESIZE / 2.0f) >= -ROOMSIZE / 2.0f)
    {
      delta -= glm::vec3(0.1f, 0.0f, 0.0f);
      moved = true;
    }
  }

  if (keys[GLFW_KEY_D])
  {
    if (cubePos.x + (CUBESIZE / 2.0f) <= ROOMSIZE / 2.0f)
    {
      delta += glm::vec3(0.1f, 0.0f, 0.0f);
      moved = true;
    }
  }

  if (keys[GLFW_KEY_Q])
  {
    if (cubePos.y + (CUBESIZE / 2.0f) <= ROOMSIZE / 2.0f)
    {
      delta += glm::vec3(0.0f, 0.1f, 0.0f);
      moved = true;
    }
  }

  if (keys[GLFW_KEY_E])
  {
    if (cubePos.y - (CUBESIZE / 2.0f) >= -ROOMSIZE / 2.0f)
    {
      delta -= glm::vec3(0.0f, 0.1f, 0.0f);
      moved = true;
    }
  }

  if (moved)
  {
    cubePos += delta;

    // update all collision planes for cube
    for (auto& cubeFace : cubeFaces)
      cubeFace.origin += delta;
  }
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/vertex_shader.glsl");

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/fragment_shader.glsl");

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
}

void setLightTargets(std::vector<GLTools::GLSpotLight>& lights,
                     const glm::vec3& target)
{
  for (auto& light : lights)
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
    distance = dot(ballPos - wall.origin, wall.normal);

    if (distance < BALLRADIUS)
    {
      ballDirection = normalize(reflect(ballDirection, wall.normal));
      ballRotationAxis = cross(ballDirection, wall.normal);
    }
  }
}

// Tells if ball lies outside area of the plane at cube face
bool isOutSidePlane(const glm::vec3& point, const CollisionPlane& plane)
{
  return dot(point - plane.origin, plane.normal) > BALLRADIUS;
}

// Tells if ball is close enough to plane from either direction
bool ballIntersectsPlane(const glm::vec3& point, const CollisionPlane& plane)
{
  return glm::abs(dot(point - plane.origin, plane.normal)) <= BALLRADIUS;
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
      normalize(reflect(ballDirection, cubeFaces[0].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[0].normal);
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[1]) &&
      in_top && in_bottom && in_front && in_back)
  {
    ballDirection =
      normalize(reflect(ballDirection, cubeFaces[1].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[1].normal);
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[2]) &&
      in_top && in_bottom && in_left && in_right)
  {
    ballDirection =
      normalize(reflect(ballDirection, cubeFaces[2].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[2].normal);
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[3]) &&
      in_top && in_bottom && in_front && in_back)
  {
    ballDirection =
      normalize(reflect(ballDirection, cubeFaces[3].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[3].normal);
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[4]) &&
      in_left && in_right && in_front && in_back)
  {
    ballDirection =
      normalize(reflect(ballDirection, cubeFaces[4].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[4].normal);
  }

  if (ballIntersectsPlane(ballPos, cubeFaces[5]) &&
      in_left && in_right && in_front && in_back)
  {
    ballDirection =
      normalize(reflect(ballDirection, cubeFaces[5].normal));
    ballRotationAxis = cross(ballDirection, cubeFaces[5].normal);
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

  // setup spotlights
  spotLights.emplace_back(glm::vec3(12.0f, 12.0f, 12.0f));
  spotLights.back().setShaderName("spotLight[0]");
  spotLights.back().setEnergy(10.0f);

  spotLights.emplace_back(glm::vec3(-12.0f, 12.0f, 12.0f));
  spotLights.back().setShaderName("spotLight[1]");
  spotLights.back().setEnergy(10.0f);

  spotLights.emplace_back(glm::vec3(-12.0f, 12.0f, -12.0f));
  spotLights.back().setShaderName("spotLight[2]");
  spotLights.back().setEnergy(10.0f);

  spotLights.emplace_back(glm::vec3(12.0f, 12.0f, -12.0f));
  spotLights.back().setShaderName("spotLight[3]");
  spotLights.back().setEnergy(10.0f);

  // get random velocity vector for ball
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-2, 2);

  ballDirection = glm::normalize(glm::vec3(dis(gen), dis(gen), dis(gen)));
  ballRotationAxis = glm::cross(ballDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

int main()
{
  init();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram);

  auto ball = std::make_unique<GLTools::GLSphere>(BALLRADIUS, 24, 24);
  ball->initialize();

  auto room = std::make_unique<GLTools::GLCube>(ROOMSIZE, true);
  room->initialize();

  auto cube = std::make_unique<GLTools::GLCube>(CUBESIZE);
  cube->initialize();

  // setup cube collision planes
  CollisionPlane cp;

  for (size_t i = 0; i < 24; i += 4)
  {
    cp.origin = room->m_vertices[i].position;
    cp.normal = room->m_vertices[i].normal;
    walls.push_back(cp);
  }

  for (size_t i = 0; i < 24; i += 4)
  {
    cp.origin = cube->m_vertices[i].position;
    cp.normal = cube->m_vertices[i].normal;
    cubeFaces.push_back(cp);
  }

  // setup textures
  auto ballTexture =
    std::make_unique<GLTools::GLTexture>("textures/ball.png");
  auto ballSpecularTexture =
    std::make_unique<GLTools::GLTexture>("textures/ball_specular.png");

  auto wallTexture =
    std::make_unique<GLTools::GLTexture>("textures/brick.jpg");
  auto wallSpecularTexture =
    std::make_unique<GLTools::GLTexture>("textures/brick_specular.jpg");

  auto cubeTexture =
    std::make_unique<GLTools::GLTexture>("textures/crate.jpg");
  auto cubeSpecularTexture =
    std::make_unique<GLTools::GLTexture>("textures/crate_specular.jpg");

  auto projection
    = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  // main loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // send info to shader
    shaderProgram->use();
    shaderProgram->setUniformValue("view", camera.m_viewMatrix);
    shaderProgram->setUniformValue("projection", projection);
    shaderProgram->setUniformValue("camPos", camera.m_position);

    shaderProgram->setUniformValue("material.diffuse", 0);
    shaderProgram->setUniformValue("material.specular", 1);
    shaderProgram->setUniformValue("material.shininess", 32.0f);

    pointLight.setShaderUniform(*shaderProgram);
    setLightUniforms(*shaderProgram);

    // draw room
    glm::mat4 roomModel;
    auto normalMatrix = glm::mat3(transpose(inverse(roomModel)));
    shaderProgram->setUniformValue("model", roomModel);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);
    wallTexture->bind(0);
    wallSpecularTexture->bind(1);
    room->draw();

    // draw cube
    glm::mat4 cubeModel;
    cubeModel = translate(cubeModel, cubePos);
    normalMatrix = glm::mat3(transpose(inverse(cubeModel)));
    shaderProgram->setUniformValue("model", cubeModel);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);
    cubeTexture->bind(0);
    cubeSpecularTexture->bind(1);
    cube->draw();

    // draw ball
    glm::mat4 ballModel;
    ballModel = translate(ballModel, ballPos);
    ballModel =
      rotate(ballModel, glm::radians(ballRotationAngle), ballRotationAxis);
    normalMatrix = glm::mat3(transpose(inverse(ballModel)));
    shaderProgram->setUniformValue("model", ballModel);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);
    ballTexture->bind(0);
    ballSpecularTexture->bind(1);
    ball->draw();

    glfwSwapBuffers(window);

    moveCube();

    // spotlights look at cube
    setLightTargets(spotLights, cubePos);

    // update ball position
    ballPos += ballDirection * BALLSPEED;
    ballRotationAngle > 360 ? ballRotationAngle = 0.0f : ballRotationAngle += 5.0f;

    // look at ball
    camera.setTarget(ballPos);

    // handle collisions
    wallCollision();
    cubeCollision();
  }

  glfwTerminate();

  return 0;
}
