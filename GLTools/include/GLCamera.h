#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;

class GLCamera
{
public:
  GLCamera(const vec3& position);
  GLCamera(const vec3& position, const vec3& target);
  GLCamera(const vec3& position, const vec3& target, const vec3& up);
  virtual ~GLCamera();

  void clearTarget();
  void orbit(const vec3& target, float angle, float radius);

  void setPosition(const vec3& position);
  const vec3& getPosition() const;

  void setTarget(const vec3& target);
  const vec3& getTarget() const;

  const vec3& getDirection() const;

  const mat4& getViewMatrix() const;
  void update();

  bool hasTarget() const;

  void dolly(float delta);
  void pan(float delta);
  void pedestal(float delta);

  void rotate(float pitch, float yaw);
  void FPSMode(float xOffset, float yOffset);

private:
  float m_pitch;
  float m_yaw;
  float m_roll;
  vec3 m_position;
  vec3 m_target;
  vec3 m_direction;
  vec3 m_up;

  mat4 m_viewMatrix;

  bool m_hasTarget = true;
};

inline void GLCamera::setPosition(const vec3& position)
{
  this->m_position = position;
}

inline const vec3& GLCamera::getPosition() const
{
  return m_position;
}

inline void GLCamera::setTarget(const vec3& target)
{
  this->m_target = target;
}

inline const vec3& GLCamera::getTarget() const
{
  return m_target;
}

inline const vec3& GLCamera::getDirection() const
{
  return m_direction;
}

inline const mat4& GLCamera::getViewMatrix() const
{
  return m_viewMatrix;
}

inline void GLCamera::update()
{
  this->m_viewMatrix = glm::lookAt(m_position, m_position - m_direction, m_up);
}

inline bool GLCamera::hasTarget() const
{
  return m_hasTarget;
}