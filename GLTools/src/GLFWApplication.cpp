#define GLEW_STATIC
#include <GL/glew.h>
#include <functional>
#include "GLFWApplication.h"

GLTools::GLFWApplication::GLFWApplication(int major, int minor)
  : m_majorVersion(major), m_minorVersion(minor)
{
}

GLTools::GLFWApplication::~GLFWApplication()
{
}

void GLTools::GLFWApplication::initGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void GLTools::GLFWApplication::createWindow(int width, int height,
                                            const std::string& title)
{
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  glewExperimental = GL_TRUE;
  glewInit();

  glViewport(0, 0, width, height);
}
