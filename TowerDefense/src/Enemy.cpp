#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Enemy.h"

#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

Enemy::Enemy(GLTools::GLModel *model, const glm::vec3& pos,
             const glm::vec3& dir)
  : m_position(pos),
    m_direction(normalize(dir)),
    m_modelMatrix(glm::mat4()),
    time(0.0f),
    explosion(false),
    explosionTime(0.0f),
    m_model(model)
{
  auto aabb = calculate_AABB();
  m_minPoint = aabb.first;
  m_maxPoint = aabb.second;
}

Enemy::~Enemy()
{}

void Enemy::damage(float damage)
{
  m_hitPoints -= damage;

  if (m_hitPoints <= 0.0f)
    m_destroyed = true;
}

bool Enemy::isDestroyed() const
{
  return m_destroyed;
}

void Enemy::draw(const GLTools::GLShaderProgram& shaderProgram)
{
  this->update();
  shaderProgram.setUniformValue("model", m_modelMatrix);
  auto normalMatrix = glm::mat3(m_modelMatrix);
  shaderProgram.setUniformValue("normalMatrix", normalMatrix);
  shaderProgram.setUniformValue("time", explosionTime);
  shaderProgram.setUniformValue("explosion", explosion);
  m_model->draw(shaderProgram);
}

std::pair<glm::vec3, glm::vec3> Enemy::calculate_AABB() const
{
  float min_x, min_y, min_z, max_x, max_y, max_z;

  min_x = min_y = min_z = 10000.0f;
  max_x = max_y = max_z = -10000.0f;

  for (const auto& mesh : m_model->m_meshes)
  {
    for (const auto& vertex : mesh.m_vertices)
    {
      auto vertexInWorldSpace = m_modelMatrix * glm::vec4(vertex.position, 1.0f);

      if (vertexInWorldSpace.x < min_x)
        min_x = vertexInWorldSpace.x;

      if (vertexInWorldSpace.x > max_x)
        max_x = vertexInWorldSpace.x;

      if (vertexInWorldSpace.y < min_y)
        min_y = vertexInWorldSpace.y;

      if (vertexInWorldSpace.y > max_y)
        max_y = vertexInWorldSpace.y;

      if (vertexInWorldSpace.z < min_z)
        min_z = vertexInWorldSpace.z;

      if (vertexInWorldSpace.z > max_z)
        max_z = vertexInWorldSpace.z;
    }
  }

  return std::make_pair(glm::vec3(min_x, min_y, min_z),
                        glm::vec3(max_x, max_y, max_z));
}

bool Enemy::isColliding(const glm::vec3& point) const
{
  auto pointInModelSpace = inverse(m_modelMatrix) * glm::vec4(point, 1.0f);

  if (pointInModelSpace.x < m_minPoint.x || pointInModelSpace.y < m_minPoint.y ||
      pointInModelSpace.z < m_minPoint.z || pointInModelSpace.x > m_maxPoint.x ||
      pointInModelSpace.y > m_maxPoint.y || pointInModelSpace.z > m_maxPoint.z)
    return false;

  return true;
}

void Enemy::update()
{
  m_progress += m_velocity;
  m_modelMatrix = translate(glm::mat4(), m_position);
  m_modelMatrix *= orientation(m_direction, -Z_AXIS);
}

glm::mat4 Enemy::getModelMatrx()
{
  return m_modelMatrix;
}
