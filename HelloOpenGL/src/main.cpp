#include <iostream>
#include "GLShaderProgram.h"
#include "GLFWApplication.h"

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
  GLShader vertexShader(GLShader::shaderType::VERTEX_SHADER);
  vertexShader.loadSource("vertex_shader.glsl");
  vertexShader.compile();
  std::cout << vertexShader.log() << std::endl;

  GLShader fragmentShader(GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader.loadSource("fragment_shader.glsl");
  fragmentShader.compile();
  std::cout << fragmentShader.log() << std::endl;

  // Shader program
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

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // STUFF --------------------------------------------------------------------
  GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  GLfloat vertices[] =
  {
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   //  0
    -0.2f, 0.2f, 0.0f, 1.0f, 1.0f,  //  1
    0.0f, 0.9f, 1.0f, 0.0f, 1.0f,   //  2
    0.2f, 0.2f, 0.0f, 1.0f, 1.0f,   //  3
    0.9f, 0.0f, 1.0f, 0.0f, 1.0f,   //  4
    0.2f, -0.2f, 0.0f, 1.0f, 1.0f,  //  5
    0.0f, -0.9f, 1.0f, 0.0f, 1.0f,  //  6
    -0.2f, -0.2f, 0.0f, 1.0f, 1.0f, //  7
    -0.9f, 0.0f, 1.0f, 0.0f, 1.0f,  //  8
  };

  GLuint indices[] =
  {
    0, 1, 2,
    2, 3, 0,
    0, 3, 4,
    4, 5, 0,
    0, 5, 6,
    6, 7, 0,
    0, 7, 8,
    8, 1, 0,
  };

  // VAO, VBO
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // --------------------------------------------------------------------------
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(app.getWindow()))
  {
    // Check and call events
    glfwPollEvents();

    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    shaderProgram.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 8 * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(app.getWindow());
  }

  glfwTerminate();

  return 0;
}
