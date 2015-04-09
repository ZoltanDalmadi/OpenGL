#pragma once
#include <glm/glm.hpp>

namespace GLTools
{

class GLCamera
{

public:
  glm::vec3 m_position;
  glm::vec3 m_up;

  glm::mat4 m_viewMatrix;

  GLCamera();
  GLCamera(const glm::vec3& position);
  virtual ~GLCamera();

private:
  virtual void update() = 0;
};

}
