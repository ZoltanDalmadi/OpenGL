#include "GLFWApplication.h"

class App : public GLTools::GLFWApplication
{
public:
  App(int, int, const char *);
  ~App() = default;

protected:
  virtual void initGL() override;
  virtual void renderGL() override;
  virtual void update() override;
  virtual void keyFunction(int key, int scancode, int action, int mod) override;
};

