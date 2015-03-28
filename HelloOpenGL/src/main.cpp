#include <iostream>
#include <vector>
#include <memory>
#include "GLShaderProgram.h"
#include "GLFWApplication.h"
#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"

using namespace GLTools;

struct Vertex
{
  glm::vec2 position;
  glm::vec3 color;
};

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode)
{
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

// SHADERS --------------------------------------------------------------------
void setupShaders(GLShaderProgram& shaderProgram)
{
  auto vertexShader =
    std::make_shared<GLShader>(GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader =
    std::make_shared<GLShader>(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("fragment_shader.glsl");
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
  GLFWApplication app(4, 3);
  app.createWindow(600, 600, "OpenGL");

  // Set the required callback functions
  glfwSetKeyCallback(app.getWindow(), key_callback);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // STUFF --------------------------------------------------------------------
  GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  std::vector<Vertex> vertices;
  Vertex vert;
  vert.position = glm::vec2(0.0f, 0.0f);
  vert.color = glm::vec3(1.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(-0.2f, 0.2f);
  vert.color = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(0.0f, 0.9f);
  vert.color = glm::vec3(1.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(0.2f, 0.2f);
  vert.color = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(0.9f, 0.0f);
  vert.color = glm::vec3(1.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(0.2f, -0.2f);
  vert.color = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(0.0f, -0.9f);
  vert.color = glm::vec3(1.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(-0.2f, -0.2f);
  vert.color = glm::vec3(0.0f, 1.0f, 1.0f);
  vertices.push_back(vert);

  vert.position = glm::vec2(-0.9f, 0.0f);
  vert.color = glm::vec3(1.0f, 0.0f, 1.0f);
  vertices.push_back(vert);

  std::vector<GLuint> indices =
  {
    0, 1, 2,
    2, 3, 0,
    0, 3, 4,
    4, 5, 0,
    0, 5, 6,
    6, 7, 0,
    0, 7, 8,
    8, 1, 0
  };

  // VAO, VBO
  GLVertexArrayObject VAO;
  GLBufferObject VBO;
  GLBufferObject EBO(GLBufferObject::BufferType::IndexBuffer);
  VBO.create();
  EBO.create();

  // --------------------------------------------------------------------------
  VAO.bind();

  VBO.bind();
  VBO.upload(vertices.data(), sizeof(Vertex) * vertices.size());

  EBO.bind();
  EBO.upload(indices.data(), sizeof(GLuint) * indices.size());

  VAO.setAttributeArray(0, 2, sizeof(Vertex), offsetof(Vertex, position));
  VAO.setAttributeArray(1, 3, sizeof(Vertex), offsetof(Vertex, color));

  VAO.unbind();

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(app.getWindow()))
  {
    // Check and call events
    glfwPollEvents();

    // Clear the colorbuffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    shaderProgram.use();

    VAO.bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    VAO.unbind();

    // Swap the buffers
    glfwSwapBuffers(app.getWindow());
  }

  glfwTerminate();

  return 0;
}
