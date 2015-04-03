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

#include "GLFPSCamera.h"
#include "GLPointLight.h"
#include "GLSkyBox.h"
#include "GLSphere.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

const size_t NUM_PLANETS = 8;

GLFWwindow *window;

glm::mat3 normalMatrix;
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

glm::vec3 randpositions[NUM_PLANETS];
bool destroyed[NUM_PLANETS];

bool cameraInCorner = false;

glm::vec3 sunPos;
glm::vec3 lastCamFront;

std::vector<const char *> skyBoxFaces;

void getRand()
{
  for (size_t i = 0; i < NUM_PLANETS; i++)
    randpositions[i] = glm::vec3(dis(gen), dis(gen), dis(gen));
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
    (GLTools::GLShader::shaderType::VERTEX_SHADER);
  cubemapVertexShader->loadSource("shaders/cubemap_vertex_shader.glsl");
  cubemapVertexShader->compile();
  std::cout << cubemapVertexShader->log() << std::endl;

  auto cubemapFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER);
  cubemapFragmentShader->loadSource("shaders/cubemap_fragment_shader.glsl");
  cubemapFragmentShader->compile();
  std::cout << cubemapFragmentShader->log() << std::endl;

  cubemapShaderProgram.create();
  cubemapShaderProgram.addShader(cubemapVertexShader);
  cubemapShaderProgram.addShader(cubemapFragmentShader);
  cubemapShaderProgram.link();
  std::cout << cubemapShaderProgram.log() << std::endl;

  auto sphereVertexShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::VERTEX_SHADER);
  sphereVertexShader->loadSource("shaders/sphere_vertex_shader.glsl");
  sphereVertexShader->compile();
  std::cout << sphereVertexShader->log() << std::endl;

  auto sphereFragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER);
  sphereFragmentShader->loadSource("shaders/sphere_fragment_shader.glsl");
  sphereFragmentShader->compile();
  std::cout << sphereFragmentShader->log() << std::endl;

  sphereShaderProgram.create();
  sphereShaderProgram.addShader(sphereVertexShader);
  sphereShaderProgram.addShader(sphereFragmentShader);
  sphereShaderProgram.link();
  std::cout << sphereShaderProgram.log() << std::endl;
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

GLuint loadCubemap(std::vector<const char *> faces)
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);

  int width, height;
  unsigned char *image;

  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  for (GLuint i = 0; i < faces.size(); i++)
  {
    image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
      GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
    );
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  return textureID;
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

  GLTools::GLShaderProgram cubemapShaderProgram;
  GLTools::GLShaderProgram sphereShaderProgram;
  setupShaders(cubemapShaderProgram, sphereShaderProgram);

  GLTools::GLSkyBox skyBox;
  skyBox.initialize();

  GLTools::GLSphere planet(1.0f, 24, 24);
  planet.initialize();

  std::vector<GLuint> planetTextures;

  GLuint cubemapTexture = loadCubemap(skyBoxFaces);

  float planetScales[NUM_PLANETS];

  planetScales[0] = 0.8f;
  planetScales[1] = 0.9f;
  planetScales[2] = 1.0f;
  planetScales[3] = 0.85f;
  planetScales[4] = 2.5f;
  planetScales[5] = 2.0f;
  planetScales[6] = 1.5;
  planetScales[7] = 1.8f;

  GLuint sunTexture = loadTexture("textures/sun.jpg");
  planetTextures.emplace_back(loadTexture("textures/mercury.jpg"));
  planetTextures.emplace_back(loadTexture("textures/venus.jpg"));
  planetTextures.emplace_back(loadTexture("textures/earth.jpg"));
  planetTextures.emplace_back(loadTexture("textures/mars.jpg"));
  planetTextures.emplace_back(loadTexture("textures/jupiter.jpg"));
  planetTextures.emplace_back(loadTexture("textures/saturn.jpg"));
  planetTextures.emplace_back(loadTexture("textures/uranus.jpg"));
  planetTextures.emplace_back(loadTexture("textures/neptune.jpg"));

  getRand();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!cameraInCorner)
      moveCamera();
    else
      moveSun();

    sphereShaderProgram.use();
    glm::mat4 sunModel;
    sunModel = glm::translate(sunModel, sunPos);
    sunModel = glm::scale(sunModel, glm::vec3(5.0f));
    glm::mat3 sunNormalMatrix = glm::mat3(glm::transpose(glm::inverse(sunModel)));
    sphereShaderProgram.setUniformValue("model", sunModel);
    sphereShaderProgram.setUniformValue("view", camera.m_viewMatrix);
    sphereShaderProgram.setUniformValue("projection", projection);
    sphereShaderProgram.setUniformValue("normalMatrix", sunNormalMatrix);

    sunLight.setShaderUniform(sphereShaderProgram);

    glBindTexture(GL_TEXTURE_2D, sunTexture);
    planet.draw();

    for (size_t i = 0; i < NUM_PLANETS; i++)
    {
      if (4.0f + planetScales[i] >= glm::distance(sunPos, randpositions[i]))
        destroyed[i] = true;
    }

    for (size_t i = 0; i < NUM_PLANETS; i++)
    {
      if (destroyed[i])
        continue;

      glm::mat4 model;
      model = glm::translate(model, randpositions[i]);
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(planetScales[i]));
      glm::mat3 planetNormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
      sphereShaderProgram.setUniformValue("model", model);
      sphereShaderProgram.setUniformValue("normalMatrix", planetNormalMatrix);
      glBindTexture(GL_TEXTURE_2D, planetTextures[i]);
      planet.draw();
    }

    glDepthFunc(GL_LEQUAL);
    cubemapShaderProgram.use();
    glm::mat4 skyBoxView = glm::mat4(glm::mat3(camera.m_viewMatrix));
    cubemapShaderProgram.setUniformValue("view", skyBoxView);
    cubemapShaderProgram.setUniformValue("projection", projection);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    skyBox.draw();
    glDepthFunc(GL_LESS);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
