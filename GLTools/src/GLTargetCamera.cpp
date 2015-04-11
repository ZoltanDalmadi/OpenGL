#include <glm/gtc/matrix_transform.hpp>
#include "GLTargetCamera.h"

GLTools::GLTargetCamera::GLTargetCamera(const glm::vec3& position)
  : GLTargetCamera(position, glm::vec3(0.0f)) {}

GLTools::GLTargetCamera::GLTargetCamera(const glm::vec3& position,
                                        const glm::vec3& target)
  : m_target(target)
{
  m_position = position;
  m_up = glm::vec3(0.0f, 1.0f, 0.0f);
  m_viewMatrix = glm::lookAt(position, target, m_up);
}

GLTools::GLTargetCamera::~GLTargetCamera()
{}

void GLTools::GLTargetCamera::orbit(const glm::vec3& target, float angle,
                                    float radius)
{
  this->m_target = target;
  float cameraX = this->m_target.x + std::sin(angle) * radius;
  float cameraZ = this->m_target.z + std::cos(angle) * radius;
  this->m_position = glm::vec3(cameraX, target.y, cameraZ);
  this->update();
}

void GLTools::GLTargetCamera::setTarget(const glm::vec3& target)
{
  this->m_target = target;
  this->update();
}

void GLTools::GLTargetCamera::moveTarget(const glm::vec3& delta)
{
  this->m_target += delta;
  this->update();
}

void GLTools::GLTargetCamera::movePosition(const glm::vec3& delta)
{
  this->m_position += delta;
  this->update();
}

void GLTools::GLTargetCamera::update()
{
  this->m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
}
