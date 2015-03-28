#include <glm/gtc/matrix_transform.hpp>
#include "GLCamera.h"

GLCamera::GLCamera(const vec3& position)
  : GLCamera(position, vec3(0.0f)) {}

GLCamera::GLCamera(const vec3& position, const vec3& target)
  : m_position(position),
    m_target(target),
    m_up(vec3(0.0f, 1.0f, 0.0f)),
    m_viewMatrix(glm::lookAt(position, target, m_up)) {}

void GLCamera::orbit(const vec3& target, float angle, float radius)
{
  this->m_target = target;
  float cameraX = this->m_target.x + std::sin(angle) * radius;
  float cameraZ = this->m_target.z + std::cos(angle) * radius;
  this->m_position = vec3(cameraX, target.y, cameraZ);
  this->update();
}

void GLCamera::moveTarget(const vec3& delta)
{
  this->m_target += delta;
  this->update();
}

void GLCamera::movePosition(const vec3& delta)
{
  this->m_position += delta;
  this->update();
}

void GLCamera::update()
{
  this->m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}