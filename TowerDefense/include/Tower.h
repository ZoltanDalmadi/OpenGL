#pragma once
#include <list>
#include "GLModel.h"
#include "Missile.h"

class Tower
{
public:
  explicit Tower(GLTools::GLModel *base, GLTools::GLModel *cannon,
                 GLTools::GLModel *missile);
  virtual ~Tower();

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& pos);

  const glm::vec3& getOffset() const;
  void setOffset(const glm::vec3& offset);

  const float& getBaseAngle() const;
  const float& getCannonAngle() const;

  void setTarget(glm::vec3 *target);
  void clearTarget();

  void shoot(const glm::vec3& pos, const glm::vec3& dir);

  void draw(const GLTools::GLShaderProgram& shaderProgram, double time);
  void update(double time);

private:
  GLTools::GLModel *m_base;
  GLTools::GLModel *m_cannon;
  GLTools::GLModel *m_missile;

  std::list<Missile> m_missiles;

  float m_baseAngle;
  float m_cannonAngle;

  glm::vec3 m_position;
  glm::vec3 *m_target;
  glm::vec3 m_offset = glm::vec3(0.33183f, 0.93760f, 0.0f);
  glm::vec3 m_cannonFront = glm::vec3(2.9f, 0.0f, 0.0f);

  glm::mat4 m_modelMatrix;
  glm::mat4 m_cannonMatrix;

  float range;

  double m_coolDown = 100.0f;
  double m_deltaTime = 0.0f;
  double m_lastShotTime;
};
