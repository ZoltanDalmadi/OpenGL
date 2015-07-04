#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace GLTools
{

class GLFWApplication
{
public:
  GLFWApplication(int, int, const char *);
  GLFWApplication(const GLFWApplication&) = delete;
  void operator=(const GLFWApplication&) = delete;
  virtual ~GLFWApplication() = default;
  int execute();
  void initialize();

private:
  void initGLFW();
  static void key_callback(GLFWwindow *, int, int, int, int);
  static void mouse_button_callback(GLFWwindow *, int, int, int);

protected:
  bool keys[1024];
  GLFWwindow *m_window;
  int m_width;
  int m_height;
  std::string m_title;

  virtual void initGL() = 0;
  virtual void renderGL() = 0;
  virtual void update() = 0;
  virtual void keyFunction(int key, int scancode, int action, int mod);
  virtual void mouseFunction(int button, int action, int mod);
};

}
