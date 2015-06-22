#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace GLTools
{

class GLFWApplication
{
public:
  GLFWApplication();
  GLFWApplication(int major, int minor);
  virtual ~GLFWApplication();

  void initGLFW();
  void createWindow(int width, int height, const std::string& title);

  GLFWwindow *getWindow() const;
  int getMajorVersion() const;
  int getMinorVersion() const;

  virtual int exec() = 0;
  virtual void init() = 0;
  virtual void render() = 0;

protected:
  GLFWwindow *m_window;
  int m_majorVersion;
  int m_minorVersion;
};

}
