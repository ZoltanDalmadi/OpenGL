#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace GLTools
{

class GLFWApplication
{
public:
  GLFWApplication(int major, int minor);
  virtual ~GLFWApplication();

  virtual void initGLFW();

  void createWindow(int width, int height, const std::string& title);

  GLFWwindow *getWindow() const;
  virtual int exec() = 0;
  virtual int keyBoardFunc() = 0;

private:
  GLFWwindow *m_window;
  int m_majorVersion;
  int m_minorVersion;
};

inline GLFWwindow *GLFWApplication::getWindow() const
{
  return m_window;
}

}
