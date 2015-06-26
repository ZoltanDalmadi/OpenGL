#pragma once
#include "GLModel.h"

class Missile
{
public:
  explicit Missile(GLTools::GLModel *model, const glm::vec3& position,
                   const glm::vec3& direction);
  virtual ~Missile();

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& pos);

  const glm::vec3& getDirection() const;
  void setDirection(const glm::vec3& dir);

  void draw(const GLTools::GLShaderProgram& shaderProgram);
  void update();

private:
  GLTools::GLModel *m_model;

  // in world space
  glm::vec3 m_position;
  glm::vec3 m_direction;

  glm::mat4 m_modelMatrix;

  float m_velocity;
  float m_maxVelocity;
  float m_acceleration;
  float m_spin;
  float m_spinSpeed;
};
