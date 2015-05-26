#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

#include "GLFPSCamera.h"
#include "GLTexture.h"
#include "GLPlane.h"
#include "GLSpotlight.h"
#include "GLFrameBufferObject.h"
#include <iostream>
#include <GLModel.h>

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

const GLuint SHADOWMAP_WIDTH = 2048;
const GLuint SHADOWMAP_HEIGHT = 2048;

float cutoff = 19.0f;
float outerCutoff = 19.0f;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 1.0f, 0.0f));

glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
glm::vec3 lightTarget(0.0f, 1.5f, 5.0f);
glm::vec3 bunnyPos(0.0f, 1.0f, 2.0f);

GLTools::GLSpotLight spotLight(lightPos);

std::vector<GLTools::GLTexture> slides;
std::vector<GLTools::GLTexture>::iterator activeSlide;

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::unique_ptr<GLTools::GLModel> model;
std::unique_ptr<GLTools::GLModel> bunny;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_F && action == GLFW_PRESS)
    if (activeSlide != slides.end())
      ++activeSlide;

  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    if (activeSlide != slides.begin())
      --activeSlide;
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

void moveLightTarget()
{
  if (keys[GLFW_KEY_I])
  {
    lightTarget += glm::vec3(0.0f, 0.02f, 0.0f);
    spotLight.setTarget(lightTarget);
  }

  if (keys[GLFW_KEY_K])
  {
    lightTarget -= glm::vec3(0.0f, 0.02f, 0.0f);
    spotLight.setTarget(lightTarget);
  }

  if (keys[GLFW_KEY_J])
  {
    lightTarget += glm::vec3(0.02f, 0.0f, 0.0f);
    spotLight.setTarget(lightTarget);
  }

  if (keys[GLFW_KEY_L])
  {
    lightTarget -= glm::vec3(0.02f, 0.0f, 0.0f);
    spotLight.setTarget(lightTarget);
  }
}

void moveBunny()
{
  if (keys[GLFW_KEY_UP])
    bunnyPos += glm::vec3(0.0f, 0.02f, 0.0f);

  if (keys[GLFW_KEY_DOWN])
    bunnyPos -= glm::vec3(0.0f, 0.02f, 0.0f);

  if (keys[GLFW_KEY_LEFT])
    bunnyPos += glm::vec3(0.02f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_RIGHT])
    bunnyPos -= glm::vec3(0.02f, 0.0f, 0.0f);

  if (keys[GLFW_KEY_PAGE_UP])
    bunnyPos += glm::vec3(0.0f, 0.0f, 0.02f);

  if (keys[GLFW_KEY_PAGE_DOWN])
    bunnyPos -= glm::vec3(0.0f, 0.0f, 0.02f);
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

  window = glfwCreateWindow(WIDTH, HEIGHT, "SpotlightShadow", nullptr, nullptr);
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
}


void renderScene(const GLTools::GLShaderProgram& shaderProgram)
{
  glm::mat4 modelModel;
  auto normalMatrix = glm::mat3(modelModel);
  shaderProgram.setUniformValue("model", modelModel);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  model->draw(shaderProgram);

  glm::mat4 bunnyModel;
  bunnyModel = translate(bunnyModel, bunnyPos);
  normalMatrix = glm::mat3(transpose(inverse(bunnyModel)));
  shaderProgram.setUniformValue("model", bunnyModel);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  bunny->draw(shaderProgram);
}

// The MAIN function
int main()
{
  init();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram);

  unsigned const pass1 = 1;
  unsigned const pass2 = 2;

  shaderProgram->use();
  shaderProgram->setUniformValue("shadowMap", 0);
  shaderProgram->setUniformValue("projectTex", 3);
  shaderProgram->setUniformValue("rect", true);

  spotLight.setTarget(lightTarget);
  spotLight.m_cutoff.second = glm::radians(cutoff);
  spotLight.m_outercutoff.second = glm::radians(outerCutoff);
  spotLight.m_ambient.second = glm::vec3(0.5f, 0.5f, 0.5f);

  model = std::make_unique<GLTools::GLModel>("models/model.obj");
  bunny = std::make_unique<GLTools::GLModel>("models/bunny.obj");

  auto depthTexture = std::make_unique<GLTools::GLTexture>();
  depthTexture->createDepthTexture(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);

  auto shadowFBO = std::make_unique<GLTools::GLFrameBufferObject>();
  shadowFBO->attachDepthTexture(*depthTexture);

  for (size_t i = 1; i < 58; i++)
  {
    auto path = "textures/dia/" + std::to_string(i) + ".jpg";
    slides.emplace_back();
    slides.back().loadTexture2DForProjection(path.c_str());
  }

  activeSlide = slides.begin();

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  auto lightProjection =
    glm::perspective(45.0f,
                     static_cast<GLfloat>(SHADOWMAP_WIDTH) /
                     static_cast<GLfloat>(SHADOWMAP_HEIGHT),
                     0.2f, 1000.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    moveCamera();
    moveLightTarget();
    moveBunny();

    // render shadow map
    auto lightView = lookAt(lightPos, lightTarget,
                            glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightSpaceMatrix = lightProjection * lightView;
    auto projScaleTrans = translate(glm::vec3(0.5f)) * scale(glm::vec3(0.8f));

    shaderProgram->setUniformValue("projectorMatrix",
                                   projScaleTrans * lightProjection * lightView);
    shaderProgram->setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
    shaderProgram->setUniformValue("viewProjection",
                                   projection * camera.m_viewMatrix);
    shaderProgram->setUniformValue("camPos", camera.m_position);
    shaderProgram->setUniformValue("lightSpaceMatrix", lightSpaceMatrix);

    spotLight.setShaderUniform(*shaderProgram);

    glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &pass1);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1);
    shadowFBO->bind();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);
    depthTexture->bind(0);
    renderScene(*shaderProgram);
    glCullFace(GL_BACK);
    glDisable(GL_POLYGON_OFFSET_FILL);
    shadowFBO->unbind();

    // back to main framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &pass2);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2);
    activeSlide->bind(3);
    renderScene(*shaderProgram);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
