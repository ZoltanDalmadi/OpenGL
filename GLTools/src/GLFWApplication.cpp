#include "GLFWApplication.h"

GLTools::GLFWApplication::GLFWApplication(int width, int height,
                                          const char *title)
  : m_width(width),
    m_height(height),
    m_title(title)
{}

int GLTools::GLFWApplication::execute()
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();

    this->renderGL();

    glfwSwapBuffers(m_window);

    this->update();
  }

  glfwTerminate();

  return 0;
}

void GLTools::GLFWApplication::initialize()
{
  this->initGLFW();
  this->initGL();
}

void GLTools::GLFWApplication::initGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  this->m_window =
    glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(this->m_window);

  glfwSwapInterval(1);

  glfwSetWindowUserPointer(this->m_window, this);

  glfwSetKeyCallback(this->m_window, &GLFWApplication::key_callback);
  glfwSetMouseButtonCallback(this->m_window,
                             &GLFWApplication::mouse_button_callback);
}

void GLTools::GLFWApplication::key_callback
(GLFWwindow *w, int key, int scd, int act, int mod)
{
  static_cast<GLFWApplication *>
  (glfwGetWindowUserPointer(w))->keyFunction(key, scd, act, mod);
}

void GLTools::GLFWApplication::mouse_button_callback
(GLFWwindow *w, int but, int act, int mod)
{
  static_cast<GLFWApplication *>
  (glfwGetWindowUserPointer(w))->mouseFunction(but, act, mod);
}

void GLTools::GLFWApplication::keyFunction
(int key, int scancode, int action, int mod)
{}

void GLTools::GLFWApplication::mouseFunction(int button, int action, int mod)
{}
