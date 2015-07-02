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

  void damage(float damage);
  bool isDestroyed() const;

  void draw(const GLTools::GLShaderProgram& shaderProgram);

  std::pair<glm::vec3, glm::vec3> calculate_AABB() const;
  bool isColliding(const glm::vec3& point) const;

  glm::vec3 m_position;
  glm::vec3 m_direction;
  float m_progress = 0.0f;
  float m_hitPoints = 100.0f;
  glm::vec3 m_minPoint;
  glm::vec3 m_maxPoint;
  glm::mat4 m_modelMatrix;
private:
  void update();
  GLTools::GLModel *m_model;


  float m_velocity = 0.001f;
  bool m_destroyed = false;
};
