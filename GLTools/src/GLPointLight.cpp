#include "GLPointLight.h"

GLTools::GLPointLight::GLPointLight
(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff,
 const glm::vec3& spec, float constant, float linear, float quad, float energy)
  : GLLight("pointLight", amb, diff, spec, energy),
    m_position("position", pos),
    m_constant("constant", constant),
    m_linear("linear", linear),
    m_quadratic("quadratic", quad)
{
  this->init();
}

GLTools::GLPointLight::~GLPointLight()
{}

void GLTools::GLPointLight::setShaderUniform
(const GLShaderProgram& program) const
{
  GLLight::setShaderUniform(program);
  program.setUniformValue(m_position_str, m_position.second);
  program.setUniformValue(m_constant_str, m_constant.second);
  program.setUniformValue(m_linear_str, m_linear.second);
  program.setUniformValue(m_quadratic_str, m_quadratic.second);
}

const glm::vec3& GLTools::GLPointLight::getPosition() const
{
  return this->m_position.second;
}

void GLTools::GLPointLight::setPosition(const glm::vec3& vec)
{
  this->m_position.second = vec;
}

const float& GLTools::GLPointLight::getConstant() const
{
  return this->m_constant.second;
}

void GLTools::GLPointLight::setConstant(const float& value)
{
  this->m_constant.second = value;
}

const float& GLTools::GLPointLight::getLinear() const
{
  return this->m_linear.second;
}

void GLTools::GLPointLight::setLinear(const float& value)
{
  this->m_linear.second = value;
}

const float& GLTools::GLPointLight::getQuadratic() const
{
  return this->m_quadratic.second;
}

void GLTools::GLPointLight::setQuadratic(const float& value)
{
  this->m_quadratic.second = value;
}

void GLTools::GLPointLight::buildShaderStrings()
{
  GLLight::buildShaderStrings();
  m_position_str = m_name + '.' + m_position.first;
  m_constant_str = m_name + '.' + m_constant.first;
  m_linear_str = m_name + '.' + m_linear.first;
  m_quadratic_str = m_name + '.' + m_quadratic.first;
}
