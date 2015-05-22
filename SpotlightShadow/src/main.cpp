#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include "GLFPSCamera.h"
#include "GLTexture.h"
#include "GLPlane.h"
#include "GLSpotlight.h"

//constants
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

float cutoff = 10.0f;
float outerCutoff = 12.0f;

GLFWwindow *window;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 0.0f, -5.0f));

glm::vec3 lightPos(0.0f, 0.0f, -10.0f);

GLTools::GLSpotLight spotLight(lightPos);

bool keys[1024];

// for mouse
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;
bool firstMouse = true;

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
    spotLight.m_cutoff.second = glm::cos(glm::radians(cutoff));
  }

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
  {
    cutoff -= 1.0f;
    spotLight.m_cutoff.second = glm::cos(glm::radians(cutoff));
  }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
  {
    outerCutoff += 1.0f;
    spotLight.m_outercutoff.second = glm::cos(glm::radians(outerCutoff));
  }

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
  {
    outerCutoff -= 1.0f;
    spotLight.m_outercutoff.second = glm::cos(glm::radians(outerCutoff));
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

  auto fragmentShader =
    std::make_shared<GLTools::GLShader>
    (GLTools::GLShader::shaderType::FRAGMENT_SHADER,
     "shaders/fragment_shader.glsl");

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
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

// The MAIN function
int main()
{
  init();

  auto shaderProgram = std::make_unique<GLTools::GLShaderProgram>();
  setupShaders(*shaderProgram);

  spotLight.setTarget(glm::vec3(0.0f));
  spotLight.m_cutoff.second = glm::cos(glm::radians(cutoff));
  spotLight.m_outercutoff.second = glm::cos(glm::radians(outerCutoff));
  spotLight.m_energy.second = 2.0f;

  auto plane = std::make_unique<GLTools::GLPlane>(20.0f, 20.0f);
  plane->initialize();

  auto model = std::make_unique<GLTools::GLMesh>("models/model.obj");

  auto wallTexture =
    std::make_unique<GLTools::GLTexture>("textures/brick.jpg");
  auto wallSpecularTexture =
    std::make_unique<GLTools::GLTexture>("textures/brick_specular.jpg");

  auto projection =
    glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 50.0f);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    moveCamera();

    // send info to shader
    shaderProgram->use();
    shaderProgram->setUniformValue("viewProjection",
                                   projection * camera.m_viewMatrix);
    shaderProgram->setUniformValue("camPos", camera.m_position);

    shaderProgram->setUniformValue("material.diffuseTex", 0);
    shaderProgram->setUniformValue("material.diffMix", 1.0f);
    shaderProgram->setUniformValue("material.specularTex", 1);
    shaderProgram->setUniformValue("material.specMix", 1.0f);
    shaderProgram->setUniformValue("material.shininess", 32.0f);

    spotLight.setShaderUniform(*shaderProgram);

    glm::mat4 planeModel;
    auto normalMatrix = glm::mat3(planeModel);
    shaderProgram->setUniformValue("model", planeModel);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);
    wallTexture->bind(0);
    wallSpecularTexture->bind(1);
    plane->draw();

    wallTexture->unbind();
    wallSpecularTexture->unbind();

    glm::mat4 modelModel;
    modelModel = translate(modelModel, glm::vec3(0.0f, 0.0f, -5.0f));
    normalMatrix = glm::mat3(transpose(inverse(modelModel)));
    shaderProgram->setUniformValue("model", modelModel);
    shaderProgram->setUniformValue("normalMatrix", normalMatrix);

    shaderProgram->setUniformValue("material.diffuse", glm::vec3(0.0f, 1.0f, 1.0f));
    shaderProgram->setUniformValue("material.diffMix", 0.0f);
    shaderProgram->setUniformValue("material.specular", glm::vec3(1.0f));
    shaderProgram->setUniformValue("material.specMix", 0.0f);
    model->draw();

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
