#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

// SHADERS --------------------------------------------------------------------

const GLchar *vertexShaderSrc =
  "#version 330 core\n"
  "layout (location = 0) in vec2 position;\n"
  "void main()\n"
  "{\n"
  "gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
  "}\0";

const GLchar *fragmentShaderSrc =
  "#version 330 core\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const GLchar *fragmentShader2Src =
  "#version 330 core\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
  "}\0";

// The MAIN function
int main()
{
  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *w = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(w);

  // Set the required callback functions
  glfwSetKeyCallback(w, key_callback);

  // Initialize GLEW to setup the OpenGL Function pointers
  glewExperimental = GL_TRUE;
  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, 800, 600);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // STUFF --------------------------------------------------------------------
  GLint success;
  GLchar infolog[512];

  // Vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
  glCompileShader(vertexShader);

  // Check compile success
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
    std::cout << "ERROR! Vertex shader compilation failed!" << std::endl
              << infolog << std::endl;
  }

  // Fragment Shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
  glCompileShader(fragmentShader);

  // Check compile success
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
    std::cout << "ERROR! Fragment shader compilation failed!" << std::endl
              << infolog << std::endl;
  }

  GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShader2Src, nullptr);
  glCompileShader(fragmentShader2);

  // Check compile success
  glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader2, 512, nullptr, infolog);
    std::cout << "ERROR! Fragment shader compilation failed!" << std::endl
              << infolog << std::endl;
  }

  // Shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check link success
  glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
    std::cout << "ERROR! Shader program linking failed!" << std::endl
              << infolog << std::endl;
  }

  GLuint shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  // Check link success
  glGetProgramiv(shaderProgram2, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shaderProgram2, 512, nullptr, infolog);
    std::cout << "ERROR! Shader program linking failed!" << std::endl
              << infolog << std::endl;
  }

  // Delete shaders, we don't need them anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(fragmentShader2);

  //GLfloat vertices[] =
  //{
  //  0.5f, 0.5f,	                    // Top Right
  //  0.5f, -0.5f,                    // Bottom Right
  //  -0.5f, -0.5f,                   // Bottom Left
  //  -0.5f, 0.5f	                    // Top Left
  //};

  GLfloat firstTriangle[] =
  {
    -0.9f, -0.5f,	// Left (X,Y)
    -0.0f, -0.5f, // Right (X,Y)
    -0.45f, 0.5f  // Top (X,Y)
  };

  GLfloat secondTriangle[] =
  {
    0.0f, -0.5f,	// Left (X,Y)
    0.9f, -0.5f, // Right (X,Y)
    0.45f, 0.5f  // Top (X,Y)
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

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle,
               GL_STATIC_DRAW);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Program loop -------------------------------------------------------------
  while (!glfwWindowShouldClose(w))
  {
    // Check and call events
    glfwWaitEvents();

    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(w);
  }

  glfwTerminate();
  return 0;
}
