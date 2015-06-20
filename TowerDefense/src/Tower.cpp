#include <glm/gtc/matrix_transform.hpp>
#include "Tower.h"

Tower::Tower(GLTools::GLModel *base)
{
  m_base = base;
}

Tower::~Tower()
{}

void Tower::setBaseModel(GLTools::GLModel *gl_model)
{
  m_base = gl_model;
}

void Tower::setTarget(glm::vec3 *target)
{
  m_target = target;
}

void Tower::clearTarget()
{
  m_target = nullptr;
  m_modelMatrix = glm::mat4(1.0f);
  m_normalMatrix = glm::mat3(1.0f);
}

void Tower::draw(const GLTools::GLShaderProgram& shaderProgram)
{
  shaderProgram.setUniformValue("model", m_modelMatrix);
  shaderProgram.setUniformValue("normalMatrix", m_normalMatrix);
  m_base->draw(shaderProgram);
}

void Tower::update()
{
  if (!m_target)
    return;

  auto v = *m_target - m_position;
  auto angle = glm::atan(v.z, v.x);
  m_modelMatrix = rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
  m_normalMatrix = glm::mat3(transpose(inverse(m_modelMatrix)));
}
