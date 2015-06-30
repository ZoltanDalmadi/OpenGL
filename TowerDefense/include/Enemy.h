#pragma once
#include <utility>
#include "GLModel.h"

class Enemy
{
public:
  explicit Enemy(GLTools::GLModel *model,
                 const glm::vec3& pos = glm::vec3(0.0f),
                 const glm::vec3& dir = glm::vec3(0.0f, 0.0f, 1.0f));

  virtual ~Enemy();

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& pos);

  const glm::vec3& getDirection() const;
  void setDirection(const glm::vec3& dir);

  void damage(float damage);
  bool isDestroyed() const;

  void draw(const GLTools::GLShaderProgram& shaderProgram);

  std::pair<glm::vec3, glm::vec3> calculate_AABB() const;
  bool isColliding(const glm::vec3& point) const;
  bool isColliding(const std::pair<glm::vec3, glm::vec3>& aabb,
                   const glm::vec3& point) const;

  float m_progress = 0.0f;
  float m_hitPoints = 100.0f;
private:
  void update();
  GLTools::GLModel *m_model;
  glm::vec3 m_position;
  glm::vec3 m_direction;

  glm::mat4 m_modelMatrix;

  float m_velocity = 0.001f;
  bool m_destroyed = false;
};
