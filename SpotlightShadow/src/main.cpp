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

float cutoff = 10.0f;
float outerCutoff = 12.0f;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 0.0f, -10.0f));

glm::vec3 lightPos(0.0f, 0.0f, -25.0f);

GLTools::GLSpotLight spotLight(lightPos);

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

std::unique_ptr<GLTools::GLPlane> plane;
std::unique_ptr<GLTools::GLModel> model;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
  {
    cutoff += 1.0f;
    spotLight.m_cutoff.second = glm::radians(cutoff);
  }

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
  {
    cutoff -= 1.0f;
    spotLight.m_cutoff.second = glm::radians(cutoff);
  }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
  {
    outerCutoff += 1.0f;
    spotLight.m_outercutoff.second = glm::radians(outerCutoff);
  }

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
  {
    outerCutoff -= 1.0f;
    spotLight.m_outercutoff.second = glm::radians(outerCutoff);
  }

  if (key == GLFW_KEY_I && action == GLFW_PRESS)
  {
    lightPos += glm::vec3(0.0f, 0.0f, 1.0f);
    spotLight.m_position.second = lightPos;
  }

  if (key == GLFW_KEY_K && action == GLFW_PRESS)
  {
    lightPos -= glm::vec3(0.0f, 0.0f, 1.0f);
    spotLight.m_position.second = lightPos;
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
  glm::mat4 planeModel;
  auto normalMatrix = glm::mat3(planeModel);
  shaderProgram.setUniformValue("model", planeModel);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  plane->draw(shaderProgram);

  glm::mat4 modelModel;
  modelModel = translate(modelModel, glm::vec3(0.0f, 0.0f, -5.0f));
  normalMatrix = glm::mat3(transpose(inverse(modelModel)));
  shaderProgram.setUniformValue("model", modelModel);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  model->draw(shaderProgram);
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

  auto wallTexture =
    std::make_shared<GLTools::GLTexture>("textures/brick.jpg");
  auto wallSpecularTexture =
    std::make_shared<GLTools::GLTexture>("textures/brick_specular.jpg");

  spotLight.setTarget(glm::vec3(0.0f));
  spotLight.m_cutoff.second = glm::radians(cutoff);
  spotLight.m_outercutoff.second = glm::radians(outerCutoff);
  spotLight.m_energy.second = 10.0f;

  model = std::make_unique<GLTools::GLModel>("models/model.obj");
  //model->m_meshes.back().m_material->m_diffuseTex = wallTexture;
  //model->m_meshes.back().m_material->m_specularTex = wallSpecularTexture;
  //model->m_meshes.back().m_material->m_diffuseTex_index = 1;
  //model->m_meshes.back().m_material->m_specularTex_index = 2;
  //model->m_meshes.back().m_material->m_diffMix = 1.0f;
  //model->m_meshes.back().m_material->m_specMix = 1.0f;

  plane = std::make_unique<GLTools::GLPlane>(20.0f, 20.0f);
  plane->initialize();
  plane->m_material->m_diffuseTex = wallTexture;
  plane->m_material->m_specularTex = wallSpecularTexture;
  plane->m_material->m_diffuseTex_index = 1;
  plane->m_material->m_specularTex_index = 2;
  plane->m_material->m_diffMix = 1.0f;
  plane->m_material->m_specular = glm::vec3(0.0f);
  plane->m_material->m_specMix = 0.2f;
  plane->m_material->m_shininess = 32.0f;

  auto depthTexture = std::make_unique<GLTools::GLTexture>();
  depthTexture->createDepthTexture(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);

  auto shadowFBO = std::make_unique<GLTools::GLFrameBufferObject>();
  shadowFBO->attachDepthTexture(*depthTexture);

  auto projectTexture =
    std::make_unique<GLTools::GLTexture>();
  projectTexture->loadTexture2DForProjection("textures/smile.jpg");

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  auto lightProjection =
    glm::perspective(45.0f,
                     static_cast<GLfloat>(SHADOWMAP_WIDTH) /
                     static_cast<GLfloat>(SHADOWMAP_HEIGHT),
                     2.0f, 50.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    moveCamera();

    // render shadow map
    auto lightView = lookAt(spotLight.m_position.second, glm::vec3(0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightSpaceMatrix = lightProjection * lightView;
    auto projScaleTrans = translate(glm::vec3(0.5f)) *
                          scale(glm::vec3(2.0f));

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
    glPolygonOffset(-0.1f, -0.1f);
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
    projectTexture->bind(3);
    renderScene(*shaderProgram);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
