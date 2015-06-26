#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Tower.h"

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

Tower::Tower(GLTools::GLModel *base, GLTools::GLModel *cannon,
             GLTools::GLModel *missile)
  : m_base(base),
    m_cannon(cannon),
    m_missile(missile),
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

void Tower::shoot(const glm::vec3& pos, const glm::vec3& dir)
{
  std::cout << "shoot!" << std::endl;
  m_missiles.emplace_back(m_missile, pos, dir);
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

  for (auto& missile : m_missiles)
  {
    missile.update();
    missile.draw(shaderProgram);
  }
}

void Tower::update(double time)
{
  if (!m_target)
    return;

  // will be the direction vector of the missile
  auto v = *m_target - m_position;
  m_baseAngle = -glm::atan(v.z, v.x);

  m_modelMatrix = translate(glm::mat4(1.0f), m_position);
  m_modelMatrix = rotate(m_modelMatrix, m_baseAngle, Y_AXIS);

  auto u = glm::vec3(glm::vec4(v - m_offset, 1.0f) * m_modelMatrix);
  m_cannonAngle =
    glm::clamp(glm::atan(u.y, u.x), glm::radians(-5.0f), glm::radians(49.0f));

  m_cannonMatrix = translate(m_modelMatrix, m_offset);
  m_cannonMatrix = rotate(m_cannonMatrix, m_cannonAngle, Z_AXIS);

  m_deltaTime += time - m_lastShotTime;

  if (m_deltaTime >= m_coolDown)
  {
    auto missilePos =
      glm::vec3(m_cannonMatrix * glm::vec4(m_cannonFront, 1.0f));
    auto missileDir = normalize(*m_target - missilePos);
    shoot(missilePos, missileDir);
    m_deltaTime = 0.0f;
    m_lastShotTime = time;
  }
}
