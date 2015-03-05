#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFWApplication.h"

GLFWApplication::GLFWApplication(int major, int minor)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

GLFWApplication::~GLFWApplication()
{
}

void GLFWApplication::createWindow(int width, int height,
                                   const std::string& title)
{
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, width, height);
}

int GLFWApplication::exec()
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();

    m_renderfunc();

    glfwSwapBuffers(m_window);
  }

  glfwTerminate();
  return 0;
}
