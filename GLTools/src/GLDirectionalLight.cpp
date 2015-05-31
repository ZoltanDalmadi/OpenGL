#include "GLDirectionalLight.h"

GLTools::GLDirectionalLight::GLDirectionalLight
(const glm::vec3& dir, const glm::vec3& amb,
 const glm::vec3& diff, const glm::vec3& spec, float energy)
  : GLLight("dirLight", amb, diff, spec, energy),
    m_direction("direction", dir)
{
  this->init();
}

GLTools::GLDirectionalLight::~GLDirectionalLight()
{}

const glm::vec3& GLTools::GLDirectionalLight::getDirection() const
{
  return this->m_direction.second;
}

void GLTools::GLDirectionalLight::setDirection(const glm::vec3& vec)
{
  this->m_direction.second = vec;
}

void GLTools::GLDirectionalLight::setShaderUniform
(const GLShaderProgram& program) const
{
  GLLight::setShaderUniform(program);
  program.setUniformValue(this->m_direction_str, this->m_direction.second);
}

void GLTools::GLDirectionalLight::buildShaderStrings()
{
  GLLight::buildShaderStrings();
  this->m_direction_str = this->m_name + '.' + this->m_direction.first;
}