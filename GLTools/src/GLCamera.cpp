#include <glm/gtc/matrix_transform.hpp>
#include "GLCamera.h"

GLCamera::GLCamera(const vec3& position)
  : GLCamera(position, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f))
{
}

GLCamera::GLCamera(const vec3& position, const vec3& target)
  : GLCamera(position, target, vec3(0.0f, 1.0f, 0.0f))
{
}

GLCamera::GLCamera(const vec3& position, const vec3& target, const vec3& up)
  : m_pitch(0),
    m_yaw(-90),
    m_roll(0),
    m_position(position),
    m_target(target),
    m_direction(glm::normalize(position - target)),
    m_up(up),
    m_viewMatrix(glm::lookAt(position, target, up)),
    m_hasTarget(true)
{
}

GLCamera::~GLCamera()
{
}

void GLCamera::clearTarget()
{
  m_target = m_position - m_direction;
  m_hasTarget = false;
  update();
}

void GLCamera::orbit(const vec3& target, float angle, float radius)
{
  m_target = target;
  m_hasTarget = true;
  float cameraX = m_target.x + std::sin(angle) * radius;
  float cameraZ = m_target.z + std::cos(angle) * radius;
  m_position = vec3(cameraX, target.y, cameraZ);
  m_direction = glm::normalize(m_position - m_target);
  m_up = vec3(0.0f, 1.0f, 0.0f);
  this->m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}

void GLCamera::dolly(float delta)
{
  m_position += m_direction * delta;
  update();
}

void GLCamera::pan(float delta)
{
  vec3 right = glm::normalize(glm::cross(m_direction, m_up));
  m_position += right * delta;
  m_target += right * delta;
  update();
}

void GLCamera::rotate(float pitch, float yaw)
{
  m_pitch += pitch;
  m_yaw += yaw;
  glm::vec3 front;
  front.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
  front.y = std::sin(glm::radians(m_pitch));
  front.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
  m_direction = glm::normalize(front);
  vec3 right = glm::normalize(glm::cross(m_direction, vec3(0.0f, 1.0f, 0.0f)));
  m_up = glm::normalize(glm::cross(right, m_direction));
  update();
}

void GLCamera::FPSMode(float xoffset, float yoffset)
{
  xoffset *= 0.05f;
  yoffset *= 0.05f;

  rotate(yoffset, xoffset);
}