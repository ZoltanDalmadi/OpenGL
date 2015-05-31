#include "GLSpotLight.h"

GLTools::GLSpotLight::GLSpotLight
(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& amb,
 const glm::vec3& diff, const glm::vec3& spec,
 float constant, float linear, float quad, float cutoff,
 float outerCutoff, float energy)
  : GLLight("spotLight", amb, diff, spec, energy),
    m_position("position", pos),
    m_direction("direction", dir),
    m_constant("constant", constant),
    m_linear("linear", linear),
    m_quadratic("quadratic", quad),
    m_cutoff("cutOff", cutoff),
    m_outercutoff("outerCutOff", outerCutoff)
{
  init();
}

GLTools::GLSpotLight::~GLSpotLight()
{}

void GLTools::GLSpotLight::setTarget(const glm::vec3& target)
{
  this->m_direction.second = normalize(target - this->m_position.second);
}

void GLTools::GLSpotLight::setShaderUniform
(const GLShaderProgram& program) const
{
  GLLight::setShaderUniform(program);
  program.setUniformValue(this->m_position_str, this->m_position.second);
  program.setUniformValue(this->m_direction_str, this->m_direction.second);
  program.setUniformValue(this->m_constant_str, this->m_constant.second);
  program.setUniformValue(this->m_linear_str, this->m_linear.second);
  program.setUniformValue(this->m_quadratic_str, this->m_quadratic.second);

  program.setUniformValue(this->m_cutoff_str, this->m_cutoff.second);
  program.setUniformValue(this->m_outercutoff_str, this->m_outercutoff.second);
}

const glm::vec3& GLTools::GLSpotLight::getPosition() const
{
  return this->m_position.second;
}

void GLTools::GLSpotLight::setPosition(const glm::vec3& vec)
{
  this->m_position.second = vec;
}

const glm::vec3& GLTools::GLSpotLight::getDirection() const
{
  return this->m_direction.second;
}

void GLTools::GLSpotLight::setDirection(const glm::vec3& vec)
{
  this->m_direction.second = vec;
}

const float& GLTools::GLSpotLight::getConstant() const
{
  return this->m_constant.second;
}

void GLTools::GLSpotLight::setConstant(const float& value)
{
  this->m_constant.second = value;
}

const float& GLTools::GLSpotLight::getLinear() const
{
  return this->m_linear.second;
}

void GLTools::GLSpotLight::setLinear(const float& value)
{
  this->m_linear.second = value;
}

const float& GLTools::GLSpotLight::getQuadratic() const
{
  return this->m_quadratic.second;
}

void GLTools::GLSpotLight::setQuadratic(const float& value)
{
  this->m_quadratic.second = value;
}

const float& GLTools::GLSpotLight::getCutoff() const
{
  return this->m_cutoff.second;
}

void GLTools::GLSpotLight::setCutoff(const float& value)
{
  this->m_cutoff.second = value;
}

const float& GLTools::GLSpotLight::getOutercutoff() const
{
  return this->m_outercutoff.second;
}

void GLTools::GLSpotLight::setOutercutoff(const float& value)
{
  this->m_outercutoff.second = value;
}

void GLTools::GLSpotLight::buildShaderStrings()
{
  GLLight::buildShaderStrings();
  this->m_position_str = this->m_name + '.' + this->m_position.first;
  this->m_direction_str = this->m_name + '.' + this->m_direction.first;
  this->m_constant_str = this->m_name + '.' + this->m_constant.first;
  this->m_linear_str = this->m_name + '.' + this->m_linear.first;
  this->m_quadratic_str = this->m_name + '.' + this->m_quadratic.first;

  this->m_cutoff_str = this->m_name + '.' + this->m_cutoff.first;
  this->m_outercutoff_str = this->m_name + '.' + this->m_outercutoff.first;
}
