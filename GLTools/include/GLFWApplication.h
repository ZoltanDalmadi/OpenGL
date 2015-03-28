#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

namespace GLTools
{

class GLFWApplication
{
public:
  GLFWApplication(int major, int minor);
  virtual ~GLFWApplication();
  void createWindow(int width, int height, const std::string& title);

  GLFWwindow *getWindow() const;

  void setRenderFunc(std::function<void()>&& func);

  int exec();

private:
  GLFWwindow *m_window;
  std::function<void()> m_renderfunc;
};

inline void GLFWApplication::setRenderFunc(std::function<void()>&& func)
{
  m_renderfunc = func;
}

inline GLFWwindow *GLFWApplication::getWindow() const
{
  return m_window;
}

}
