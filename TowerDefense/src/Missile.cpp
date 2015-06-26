#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Missile.h"

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)

Missile::Missile(GLTools::GLModel *model, const glm::vec3& position,
                 const glm::vec3& direction)
  : m_model(model),
    m_position(position),
    m_direction(direction),
    m_velocity(0.0f),
    m_maxVelocity(1.0f),
    m_acceleration(0.01f),
    m_spin(0.0f),
    m_spinSpeed(glm::radians(2.0f))
{}

Missile::~Missile()
{}

const glm::vec3& Missile::getPosition() const
{
  return m_position;
}

void Missile::setPosition(const glm::vec3& pos)
{
  m_position = pos;
  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_modelMatrix = rotate(m_modelMatrix, m_spin, m_direction);
}

const glm::vec3& Missile::getDirection() const
{
  return m_direction;
}

void Missile::setDirection(const glm::vec3& dir)
{
  m_direction = dir;
}

void Missile::draw(const GLTools::GLShaderProgram& shaderProgram)
{
  this->update();
  shaderProgram.setUniformValue("model", m_modelMatrix);
  auto normalMatrix = glm::mat3(m_modelMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  m_model->draw(shaderProgram);
}

void Missile::update()
{
  if (m_velocity < m_maxVelocity)
    m_velocity += m_acceleration;

  m_position += m_direction * m_velocity;
  m_spin += m_spinSpeed;

  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_modelMatrix = rotate(m_modelMatrix, m_spin, m_direction);
  m_modelMatrix *= orientation(m_direction, X_AXIS);
}
