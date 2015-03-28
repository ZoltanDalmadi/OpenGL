#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <memory>

#include "GLShaderProgram.h"
#include "GLFPSCamera.h"

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

GLFWwindow *window;
glm::mat4 model;
glm::mat4 projection;

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
};

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-3, 3);

glm::vec3 randpositions[20];

double lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;

GLFPSCamera camera(glm::vec3(0.0f, 0.0f, -10.0f));

bool keys[1024];

void getRand()
{
  for (size_t i = 0; i < 20; i++)
    randpositions[i] = glm::vec3(dis(gen), dis(gen), dis(gen));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    getRand();

  if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
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
  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Cube", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLShader>(GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("vertex_shader.glsl");
  vertexShader->compile();

  auto fragmentShader =
    std::make_shared<GLShader>(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("fragment_shader.glsl");
  fragmentShader->compile();

  // Shader program
  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
}

// The MAIN function
int main()
{
  init();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // STUFF --------------------------------------------------------------------
  GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  std::vector<Vertex> vertices;
  Vertex vert;

  vert.position = glm::vec3(-0.5f, -0.5f, 0.5f);
  vert.color = glm::vec3(1.0f, 0.0f, 0.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(0.5f, -0.5f, 0.5f);
  vert.color = glm::vec3(0.0f, 1.0f, 0.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(-0.5f, 0.5f, 0.5f);
  vert.color = glm::vec3(0.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(0.5f, 0.5f, 0.5f);
  vert.color = glm::vec3(1.0f, 1.0f, 0.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(-0.5f, -0.5f, -0.5f);
  vert.color = glm::vec3(1.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(0.5f, -0.5f, -0.5f);
  vert.color = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(-0.5f, 0.5f, -0.5f);
  vert.color = glm::vec3(1.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec3(0.5f, 0.5f, -0.5f);
  vert.color = glm::vec3(0.0f, 0.0f, 0.0f);
  vertices.push_back(vert);

  std::vector<GLuint> indices =
  {
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
  };

  // VAO, VBO
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(offsetof(Vertex, color)));
  glBindVertexArray(0);

  projection = glm::perspective(45.0f, WIDTH / (HEIGHT * 1.0f), 0.1f, 100.0f);

  GLint mLoc = glGetUniformLocation(shaderProgram.ID(), "model");
  GLint vLoc = glGetUniformLocation(shaderProgram.ID(), "view");
  GLint pLoc = glGetUniformLocation(shaderProgram.ID(), "projection");

  getRand();

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (keys[GLFW_KEY_W])
      camera.move(GLFPSCamera::Direction::FORWARD);

    if (keys[GLFW_KEY_S])
      camera.move(GLFPSCamera::Direction::BACKWARD);

    if (keys[GLFW_KEY_A])
      camera.move(GLFPSCamera::Direction::LEFT);

    if (keys[GLFW_KEY_D])
      camera.move(GLFPSCamera::Direction::RIGHT);

    if (keys[GLFW_KEY_SPACE])
      camera.move(GLFPSCamera::Direction::UP);

    if (keys[GLFW_KEY_LEFT_CONTROL])
      camera.move(GLFPSCamera::Direction::DOWN);

    // Render
    shaderProgram.use();

    glBindVertexArray(VAO);

    for (size_t i = 0; i < 20; i++)
    {
      glm::mat4 model;
      model = glm::translate(model, randpositions[i]);
      model = glm::rotate(model, 20.0f * i, randpositions[i]);
      glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices.size()),
                     GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(camera.m_viewMatrix));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}
