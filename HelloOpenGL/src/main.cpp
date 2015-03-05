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
  app.createWindow(800, 600, "OpenGL");

  // Set the required callback functions
  glfwSetKeyCallback(app.getWindow(), key_callback);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // STUFF --------------------------------------------------------------------
  GLShaderProgram shaderProgram;
  setupShaders(shaderProgram);

  //GLfloat vertices[] =
  //{
  //  0.5f, 0.5f,	                    // Top Right
  //  0.5f, -0.5f,                    // Bottom Right
  //  -0.5f, -0.5f,                   // Bottom Left
  //  -0.5f, 0.5f	                    // Top Left
  //};

  GLfloat firstTriangle[] =
  {
    -0.9f, -0.5f,	1.0f, 0.0f, 0.0f, // Left (X, Y, R, G, B)
    -0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // Right (X, Y, R, G, B)
    -0.45f, 0.5f, 0.0f, 0.0f, 1.0f  // Top (X, Y, R, G, B)
  };

  GLfloat secondTriangle[] =
  {
    0.0f, -0.5f, 0.0f, 1.0f, 0.0f,	// Left (X, Y, R, G, B)
    0.9f, -0.5f, 1.0f, 0.0f, 0.0f,  // Right (X, Y, R, G, B)
    0.45f, 0.5f, 0.0f, 0.0f, 1.0f   // Top (X, Y, R, G, B)
  };

  //GLuint indices[] =
  //{
  //  0, 1, 3,	                      // First Triangle
  //  1, 2, 3	                        // Second Triangle
  //};

  // VAO, VBO
  GLuint VAO, VAO2, VBO, VBO2;
  glGenVertexArrays(1, &VAO);
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &VBO2);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // --------------------------------------------------------------------------
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // --------------------------------------------------------------------------
  glBindVertexArray(VAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle,
               GL_STATIC_DRAW);

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
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //auto time = glfwGetTime() * 4;
    //auto val = static_cast<GLfloat>((std::cos(time) + 1) / 2.0f);

    //if (val < 10e-4)
    //  val = 0.0;

    //std::cout << val << std::endl;

    //GLint vertexColorLocation = glGetUniformLocation(shaderProgram2, "vertexColor");
    //glUniform4f(vertexColorLocation, val, val, val, 1.0f);

    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(app.getWindow());
  }

  glfwTerminate();

  return 0;
}
