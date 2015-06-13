#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "GLShaderProgram.h"

//A g�rbe kipr�b�l�sa, illettve t�rgyak g�rbe ment�n l�v� mozgat�s�nak kipr�b�l�s�ra k�sz�lt project

using namespace GLTools;

// CONSTANTS --------------------------------------------------------------
const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;


// GLOBAL VARIABLES -------------------------------------------------------
GLFWwindow *window;

void init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Curves", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void setupShaders(GLShaderProgram& shaderProgram)
{
  auto vertexShader = std::make_shared<GLShader>
                      (GLShader::shaderType::VERTEX_SHADER);
  vertexShader->loadSource("vertex_shader.glsl");
  vertexShader->compile();
  std::cout << vertexShader->log() << std::endl;

  auto fragmentShader = std::make_shared<GLShader>
                        (GLShader::shaderType::FRAGMENT_SHADER);
  fragmentShader->loadSource("fragment_shader.glsl");
  fragmentShader->compile();
  std::cout << fragmentShader->log() << std::endl;

  shaderProgram.create();
  shaderProgram.addShader(vertexShader);
  shaderProgram.addShader(fragmentShader);
  shaderProgram.link();
  std::cout << shaderProgram.log() << std::endl;
}

int main()
{

  init();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
