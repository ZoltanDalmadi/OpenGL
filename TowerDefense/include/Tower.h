#pragma once
#include <list>
#include "GLModel.h"
#include "Missile.h"

class Tower
{
public:
  explicit Tower(GLTools::GLModel *base, GLTools::GLModel *cannon,
                 GLTools::GLModel *missile);

  explicit Tower(GLTools::GLModel *base, GLTools::GLModel *cannon,
                 GLTools::GLModel *missile, const glm::vec3& pos);

  virtual ~Tower();

  std::list<Missile> m_missiles;
  glm::vec3 *m_target = nullptr;

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& pos);

  const glm::vec3& getOffset() const;
  void setOffset(const glm::vec3& offset);

  float getCannonLength() const;
  void setCannonLength(float cannon_length);

  void setTarget(glm::vec3 *target);
  void clearTarget();

  float getRange() const;
  void setRange(float range);

  float getDamage() const;
  void setDamage(float damage);

  void shoot(const glm::vec3& pos, const glm::vec3& dir);
  void draw(const GLTools::GLShaderProgram& shaderProgram);
  void draw(const GLTools::GLShaderProgram& shaderProgram, double time);

private:
  void update(double time);

  GLTools::GLModel *m_base;
  GLTools::GLModel *m_cannon;
  GLTools::GLModel *m_missile;

  glm::vec3 m_position;
  glm::vec3 m_offset = glm::vec3(0.33183f, 0.93760f, 0.0f);
  float m_cannonLength = 2.9f;

  float m_baseAngle;
  float m_cannonAngle;

  glm::mat4 m_modelMatrix;
  glm::mat4 m_cannonMatrix;

  float m_range = 20.0f;
  float m_damage = 25.0f;

  double m_coolDown = 100.0f;
  double m_deltaTime = 0.0f;
  double m_lastShotTime;
};
