#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <list>

#include "GLFPSCamera.h"
#include "GLPointLight.h"
#include "GLSkyBox.h"
#include "GLSphere.h"
#include "GLTexture.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;
const size_t NUM_PLANETS = 8;

GLFWwindow *window;

glm::mat4 projection;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 0.0f, -5.0f));

GLTools::GLPointLight sunLight;

bool keys[1024];

double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;

bool firstMouse = true;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-10, 10);

bool cameraInCorner = false;

glm::vec3 sunPos;
glm::vec3 lastCamFront;

std::vector<const char *> skyBoxFaces;

struct Planet
{
  glm::vec3 pos;
  float scale;
  std::unique_ptr<GLTools::GLTexture> texture;
};

std::list<Planet> planets(NUM_PLANETS);

void getRand()
{
  for (auto it = planets.begin(); it != planets.end(); ++it)
    it->pos = glm::vec3(dis(gen), dis(gen), dis(gen));
}

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
  {
    if (cameraInCorner)
    {
      cameraInCorner = false;
      glfwSetCursorPos(window, lastX, lastY);
      camera.m_position = sunPos;
      camera.m_front = lastCamFront;
      camera.m_viewMatrix =
        glm::lookAt(camera.m_position, camera.m_position - camera.m_front, camera.m_up);
    }
    else
    {
      cameraInCorner = true;
      lastCamFront = camera.m_front;
      camera.m_position = glm::vec3(20.0f, 20.0f, 20.0f);
      camera.m_front = glm::normalize(camera.m_position);
      camera.m_viewMatrix =
        glm::lookAt(camera.m_position, camera.m_position - camera.m_front, camera.m_up);
    }
  }
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

  sunPos = camera.m_position;
  sunLight.m_position.second = sunPos;
}

void moveSun()
{
  if (keys[GLFW_KEY_W])
    sunPos -= glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_S])
    sunPos += glm::vec3(0.0f, 0.0f, 0.1f);

  if (keys[GLFW_KEY_A])
    sunPos -= glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_D])
    sunPos += glm::vec3(0.1f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_SPACE])
    sunPos += glm::vec3(0.0f, 0.1f, 0.0f);

  if (keys[GLFW_KEY_LEFT_CONTROL])
    sunPos -= glm::vec3(0.0f, 0.1f, 0.0f);

  sunLight.m_position.second = sunPos;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  if (!cameraInCorner)
  {
    double xoffset = xpos - lastX;
    double yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;
    camera.rotate(static_cast<float>(xoffset), static_cast<float>(yoffset));
  }
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLTools::GLShaderProgram& cubemapShaderProgram,
                  GLTools::GLShaderProgram& sphereShaderProgram)
{
  auto cubemapVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/cubemap_vertex_shader.glsl");

  auto cubemapFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/cubemap_fragment_shader.glsl");

  cubemapShaderProgram.create();
  cubemapShaderProgram.addShader(cubemapVertexShader);
  cubemapShaderProgram.addShader(cubemapFragmentShader);
  cubemapShaderProgram.link();

  auto sphereVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER,
     "shaders/sphere_vertex_shader.glsl");

  auto sphereFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/sphere_fragment_shader.glsl");

  sphereShaderProgram.create();
  sphereShaderProgram.addShader(sphereVertexShader);
  sphereShaderProgram.addShader(sphereFragmentShader);
  sphereShaderProgram.link();
}

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "HomeWork1", nullptr, nullptr);
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
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  skyBoxFaces.push_back("textures/nebula_right.png");
  skyBoxFaces.push_back("textures/nebula_left.png");
  skyBoxFaces.push_back("textures/nebula_top.png");
  skyBoxFaces.push_back("textures/nebula_bottom.png");
  skyBoxFaces.push_back("textures/nebula_back.png");
  skyBoxFaces.push_back("textures/nebula_front.png");

  sunLight.m_energy.second = 5.0f;
  sunLight.buildShaderStrings();

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 5.0f, 100.0f);
}

// The MAIN function
int main()
{
  init();

  auto cubemapShaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  auto sphereShaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*cubemapShaderProgram, *sphereShaderProgram);

  auto skyBox = std::make_unique<GLTools::GLSkyBox>();
  skyBox->initialize();

  auto planet = std::make_unique<GLTools::GLSphere>(1.0f, 24, 24);
  planet->initialize();

  auto cubemapTexture = std::make_unique<GLTools::GLTexture>(skyBoxFaces.data());

  getRand();

  auto planetIter = planets.begin();

  planetIter->scale = 0.8f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/mercury.jpg");
  planetIter++;

  planetIter->scale = 0.9f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/venus.jpg");
  planetIter++;

  planetIter->scale = 1.0f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/earth.jpg");
  planetIter++;

  planetIter->scale = 0.85f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/mars.jpg");
  planetIter++;

  planetIter->scale = 2.5f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/jupiter.jpg");
  planetIter++;

  planetIter->scale = 2.0f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/saturn.jpg");
  planetIter++;

  planetIter->scale = 1.5;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/uranus.jpg");
  planetIter++;

  planetIter->scale = 1.8f;
  planetIter->texture =
    std::make_unique<GLTools::GLTexture>("textures/neptune.jpg");

  auto sunTexture = std::make_unique<GLTools::GLTexture>("textures/sun.jpg");

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!cameraInCorner)
      moveCamera();
    else
      moveSun();

    sphereShaderProgram->use();
    glm::mat4 sunModel;
    sunModel = glm::translate(sunModel, sunPos);
    sunModel = glm::scale(sunModel, glm::vec3(5.0f));
    glm::mat3 sunNormalMatrix = glm::mat3(glm::transpose(glm::inverse(sunModel)));
    sphereShaderProgram->setUniformValue("model", sunModel);
    sphereShaderProgram->setUniformValue("view", camera.m_viewMatrix);
    sphereShaderProgram->setUniformValue("projection", projection);
    sphereShaderProgram->setUniformValue("normalMatrix", sunNormalMatrix);

    sunLight.setShaderUniform(*sphereShaderProgram);

    sunTexture->bind();
    planet->draw();

    for (auto it = planets.begin(); it != planets.end();)
    {
      if (4.0f + it->scale >= glm::distance(sunPos, it->pos))
        it = planets.erase(it);
      else
        ++it;
    }

    if (planets.empty())
    {
      std::cout << "You have destroyed all the planets!" << std::endl;
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    for (auto it = planets.cbegin(); it != planets.cend(); ++it)
    {
      glm::mat4 model;
      model = glm::translate(model, it->pos);
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(it->scale));
      glm::mat3 planetNormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
      sphereShaderProgram->setUniformValue("model", model);
      sphereShaderProgram->setUniformValue("normalMatrix", planetNormalMatrix);
      it->texture->bind();
      planet->draw();
    }

    glDepthFunc(GL_LEQUAL);
    cubemapShaderProgram->use();
    glm::mat4 skyBoxView = glm::mat4(glm::mat3(camera.m_viewMatrix));
    cubemapShaderProgram->setUniformValue("view", skyBoxView);
    cubemapShaderProgram->setUniformValue("projection", projection);
    cubemapTexture->bind();
    skyBox->draw();
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
