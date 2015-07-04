#include <iostream>
#include "App.h"

App::App(int width, int height, const char *title)
  : GLFWApplication(width, height, title)
{}

void App::initGL()
{
  glewExperimental = GL_TRUE;
  glewInit();

  std::cout << GLEW_VERSION << std::endl;

  glViewport(0, 0, m_width, m_height);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void App::renderGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void App::update()
{
  if (keys[GLFW_KEY_E])
    std::cout << "Pressed E" << std::endl;
}

void App::keyFunction(int key, int scancode, int action, int mod)
{
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(m_window, GL_TRUE);
}