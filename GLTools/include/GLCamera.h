#pragma once
#include <glm/glm.hpp>

using glm::vec3;

class GLCamera
{
public:
  vec3 m_position;
  vec3 m_target;
  vec3 m_up;

  glm::mat4 m_viewMatrix;

  GLCamera(const vec3& position);
  GLCamera(const vec3& position, const vec3& target);

  void orbit(const vec3& target, float angle, float radius);

  void moveTarget(const vec3& delta);
  void movePosition(const vec3& delta);

private:
  void update();
};
