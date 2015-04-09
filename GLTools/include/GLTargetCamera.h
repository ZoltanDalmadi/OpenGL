#pragma once
#include "GLCamera.h"

namespace GLTools
{

class GLTargetCamera : public GLCamera
{
public:
  glm::vec3 m_target;

  GLTargetCamera(const glm::vec3& position);
  GLTargetCamera(const glm::vec3& position, const glm::vec3& target);
  virtual ~GLTargetCamera();

  void orbit(const glm::vec3& target, float angle, float radius);

  void moveTarget(const glm::vec3& delta);
  void movePosition(const glm::vec3& delta);

private:
  virtual void update();
};

}
