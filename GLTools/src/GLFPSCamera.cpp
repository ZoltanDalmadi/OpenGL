#include <glm/gtc/matrix_transform.hpp>
#include "GLFPSCamera.h"

GLTools::GLFPSCamera::GLFPSCamera(const glm::vec3& position)
  : m_pitch(0),
    m_yaw(-90.0f),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f))
{
  m_position = position;
  this->update();
}

GLTools::GLFPSCamera::~GLFPSCamera()
{}

void GLTools::GLFPSCamera::move(GLFPSCamera::Direction dir)
{
  switch (dir)
  {
  case GLFPSCamera::Direction::FORWARD:
    this->m_position -= this->m_front * this->m_speed;
    break;

  case GLFPSCamera::Direction::BACKWARD:
    this->m_position += this->m_front * this->m_speed;
    break;

  case GLFPSCamera::Direction::LEFT:
    this->m_position += this->m_right * this->m_speed;
    break;

  case GLFPSCamera::Direction::RIGHT:
    this->m_position -= this->m_right * this->m_speed;
    break;

  case GLFPSCamera::Direction::UP:
    this->m_position += this->m_up * this->m_speed;
    break;

  case GLFPSCamera::Direction::DOWN:
    this->m_position -= this->m_up * this->m_speed;
    break;
  }

  this->update();
}

void GLTools::GLFPSCamera::rotate(float xoffset, float yoffset, bool constrain)
{
  xoffset *= this->m_sensivity;
  yoffset *= this->m_sensivity;

  this->m_yaw += xoffset;
  this->m_pitch += yoffset;

  if (constrain)
  {
    if (this->m_pitch > 89.0f)
      this->m_pitch = 89.0f;

    if (this->m_pitch < -89.0f)
      this->m_pitch = -89.0f;
  }

  this->update();
}

void GLTools::GLFPSCamera::update()
{
  glm::vec3 front;
  front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  front.y = std::sin(glm::radians(m_pitch));
  front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
  m_up = glm::normalize(glm::cross(m_right, m_front));
  m_viewMatrix = glm::lookAt(m_position, m_position - m_front, m_up);
}