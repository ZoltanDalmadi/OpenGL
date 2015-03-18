#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "GLShaderProgram.h"

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

GLFWwindow *window;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    model = glm::rotate(model, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    model = glm::rotate(model, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));

  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    model = glm::rotate(model, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));

  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    model = glm::rotate(model, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));

  if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.1f));

  if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.1f));

  if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::translate(view, glm::vec3(0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::translate(view, glm::vec3(-0.1f, 0.0f, 0.0f));

  if (key == GLFW_KEY_LEFT && mods == GLFW_MOD_SHIFT &&
      (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::rotate(view, -0.01f, glm::vec3(1.0f, 1.0f, 0.0f));

  if (key == GLFW_KEY_RIGHT && mods == GLFW_MOD_SHIFT &&
      (action == GLFW_PRESS || action == GLFW_REPEAT))
    view = glm::rotate(view, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
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
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram)
{
  GLShader vertexShader(GLShader::shaderType::VERTEX_SHADER);
  vertexShader.loadSource("vertex_shader.glsl");
  vertexShader.compile();

  GLShader fragmentShader(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader.loadSource("fragment_shader.glsl");
  fragmentShader.compile();

  // Shader program
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
}

// The MAIN function
int main()
{
  init();

  glfwSetKeyCallback(window, key_callback);

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

  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(45.0f, WIDTH / static_cast<float>(HEIGHT),
                                0.1f, 100.0f);

  GLint mLoc = glGetUniformLocation(shaderProgram.ID(), "model");
  GLint vLoc = glGetUniformLocation(shaderProgram.ID(), "view");
  GLint pLoc = glGetUniformLocation(shaderProgram.ID(), "projection");

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    shaderProgram.use();
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}
