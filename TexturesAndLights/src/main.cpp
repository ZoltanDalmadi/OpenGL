#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include <iostream>
#include <vector>

#include "GLShaderProgram.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLFPSCamera.h"
#include "GLDirectionalLight.h"
#include "GLPointLight.h"
#include "GLSpotLight.h"
#include "GLCube.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;

glm::mat4 model;
glm::mat4 cubeModel;
glm::mat3 normalMatrix;
glm::mat4 projection;

GLTools::GLFPSCamera camera(glm::vec3(0.0f, 0.5f, -1.0f));

//GLTools::GLDirectionalLight light(glm::vec3(-1.2f, -0.8f, -2.5f),
//                                  glm::vec3(0.2f, 0.2f, 0.2f),
//                                  glm::vec3(0.8f, 0.8f, 0.8f),
//                                  glm::vec3(1.0f, 1.0f, 1.0f),
//                                  5.0f);
//GLTools::GLSpotLight light(glm::vec3(0.0f, 1.0f, 0.0f));

GLTools::GLPointLight pointLight1(glm::vec3(5.0f, 1.0f, 0.0f));
GLTools::GLPointLight pointLight2(glm::vec3(-5.0f, 1.0f, 0.0f));
GLTools::GLSpotLight spotLight1(glm::vec3(-8.0f, 8.0f, 8.0f));
GLTools::GLSpotLight spotLight2(glm::vec3(8.0f, 8.0f, 8.0f));
GLTools::GLSpotLight spotLight3(glm::vec3(-8.0f, 8.0f, -8.0f));
GLTools::GLSpotLight spotLight4(glm::vec3(8.0f, 8.0f, -8.0f));

GLTools::GLCube cube(2.0f);

glm::vec3 lightPosition(0.0f, 1.0f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

bool keys[1024];
double lastX = WIDTH / 2.0f;
double lastY = HEIGHT / 2.0f;

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

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

  //if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second += glm::vec3(0.1f, 0.0f, 0.0f);

  //if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second -= glm::vec3(0.1f, 0.0f, 0.0f);

  //if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second += glm::vec3(0.0f, 0.1f, 0.0f);

  //if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second -= glm::vec3(0.0f, 0.1f, 0.0f);

  //if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second += glm::vec3(0.0f, 0.0f, 0.1f);

  //if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_position.second -= glm::vec3(0.0f, 0.0f, 0.1f);

  //if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_energy.second += 0.1f;

  //if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
  //  light.m_energy.second -= 0.1f;
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

  double xoffset = xpos - lastX;
  double yoffset = ypos - lastY;
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

  window = glfwCreateWindow(WIDTH, HEIGHT, "TexturesAndLights", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //camera.m_speed = 0.04f;
  pointLight1.m_name = "pointLight[0]";
  pointLight1.buildShaderStrings();
  pointLight2.m_name = "pointLight[1]";
  pointLight2.buildShaderStrings();
  spotLight1.m_name = "spotLight[0]";
  spotLight1.buildShaderStrings();
  spotLight2.m_energy.second = 3.0f;
  spotLight2.m_name = "spotLight[1]";
  spotLight2.buildShaderStrings();
  spotLight3.m_name = "spotLight[2]";
  spotLight3.buildShaderStrings();
  spotLight4.m_name = "spotLight[3]";
  spotLight4.buildShaderStrings();

  cube.initialize();
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

  // Shader program
  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

// The MAIN function
int main()
{
  init();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // STUFF --------------------------------------------------------------------
  GLTools::GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  std::vector<Vertex> vertices;
  Vertex vert;

  vert.position = glm::vec3(-10.0f, 0.0f, 10.0f);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(-10.0f, 0.0f, -10.0f);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(10.0f, 0.0f, 10.0f);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(10.0f, 0.0f, -10.0f);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  vertices.push_back(vert);

  std::vector<GLuint> indices { 0, 2, 1, 3 };

  GLuint texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  int width, height;
  unsigned char *image =
    SOIL_load_image("textures/concrete.jpg", &width, &height, 0, SOIL_LOAD_RGB);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);

  GLuint texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  image =
    SOIL_load_image("textures/awesomeface.png", &width, &height, 0,
                    SOIL_LOAD_RGB);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);

  // VAO, VBO
  GLTools::GLVertexArrayObject VAO;
  VAO.create();

  GLTools::GLBufferObject VBO;
  GLTools::GLBufferObject EBO(GLTools::GLBufferObject::BufferType::IndexBuffer);
  VBO.create();
  EBO.create();

  // --------------------------------------------------------------------
  VAO.bind();

  VBO.bind();
  VBO.upload(vertices.data(), vertices.size() * sizeof(Vertex));

  EBO.bind();
  EBO.upload(indices.data(), indices.size() * sizeof(GLuint));

  VAO.setAttributeArray(0, 3, sizeof(Vertex));
  VAO.setAttributeArray(1, 3, sizeof(Vertex), offsetof(Vertex, normal));
  VAO.setAttributeArray(2, 2, sizeof(Vertex), offsetof(Vertex, texCoords));

  // --------------------------------------------------------------------
  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);

  cubeModel = glm::translate(cubeModel, glm::vec3(6.0f, 3.0f, 6.0f));

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    moveCamera();

    // Render
    shaderProgram.use();
    shaderProgram.setUniformValue("model", model);
    shaderProgram.setUniformValue("view", camera.m_viewMatrix);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("normalMatrix",
                                  glm::mat3(glm::transpose(glm::inverse(model))));
    shaderProgram.setUniformValue("camPos", camera.m_position);
    shaderProgram.setUniformValue("material.diffuse1", 0);
    shaderProgram.setUniformValue("material.diffuse2", 1);
    shaderProgram.setUniformValue("material.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue("material.shininess", 32.0f);

    pointLight1.setShaderUniform(shaderProgram);
    pointLight2.setShaderUniform(shaderProgram);
    spotLight1.setShaderUniform(shaderProgram);
    spotLight2.setShaderUniform(shaderProgram);
    spotLight3.setShaderUniform(shaderProgram);
    spotLight4.setShaderUniform(shaderProgram);

    VAO.bind();

    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    shaderProgram.setUniformValue("model", cubeModel);

    cube.m_VAO.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cube.m_indices.size()),
                   GL_UNSIGNED_INT, 0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
