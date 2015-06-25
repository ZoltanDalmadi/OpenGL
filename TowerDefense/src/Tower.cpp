#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Tower.h"

Tower::Tower(GLTools::GLModel *base, GLTools::GLModel *cannon)
  : m_base(base),
    m_cannon(cannon),
    m_baseAngle(0.0f),
    m_cannonAngle(0.0f),
    m_modelMatrix(translate(glm::mat4(1.0f), m_position)),
    m_cannonMatrix(translate(m_modelMatrix, m_offset))
{}

Tower::~Tower()
{}

const glm::vec3& Tower::getPosition() const
{
  return m_position;
}

void Tower::setPosition(const glm::vec3& pos)
{
  m_position = pos;
  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_cannonMatrix = translate(m_modelMatrix, m_offset);
}

const glm::vec3& Tower::getOffset() const
{
  return m_offset;
}

void Tower::setOffset(const glm::vec3& offset)
{
  m_offset = offset;
}

const float& Tower::getBaseAngle() const
{
  return m_baseAngle;
}

const float& Tower::getCannonAngle() const
{
  return m_cannonAngle;
}

void Tower::setTarget(glm::vec3 *target)
{
  m_target = target;
}

void Tower::clearTarget()
{
  m_target = nullptr;
  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_cannonMatrix = translate(m_modelMatrix, m_offset);
}

void Tower::shoot()
{
  std::cout << "shoot!" << std::endl;
}

void Tower::draw(const GLTools::GLShaderProgram& shaderProgram, double time)
{
  this->update(time);

  shaderProgram.setUniformValue("model", m_modelMatrix);
  auto normalMatrix = glm::mat3(m_modelMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  m_base->draw(shaderProgram);

  shaderProgram.setUniformValue("model", m_cannonMatrix);
  normalMatrix = glm::mat3(m_cannonMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  m_cannon->draw(shaderProgram);
}

void Tower::update(double time)
{
  if (!m_target)
    return;

  auto v = *m_target - m_position;
  m_baseAngle = -glm::atan(v.z, v.x);

  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_modelMatrix = rotate(m_modelMatrix, m_baseAngle, glm::vec3(0.0f, 1.0f, 0.0f));

  auto u = glm::vec3(glm::vec4(v - m_offset, 1.0f) * m_modelMatrix);
  m_cannonAngle = glm::clamp(glm::atan(u.y, u.x), glm::radians(-5.0f),
                             glm::radians(49.0f));

  m_cannonMatrix = translate(m_modelMatrix, m_offset);
  m_cannonMatrix = rotate(m_cannonMatrix, m_cannonAngle, glm::vec3(0.0f, 0.0f,
                          1.0f));

  m_deltaTime += time - m_lastShotTime;

  if (m_deltaTime >= m_coolDown)
  {
    shoot();
    m_deltaTime = 0.0f;
    m_lastShotTime = time;
  }
}
