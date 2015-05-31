#include "GLLight.h"
#include <utility>

GLTools::GLLight::GLLight(
  const std::string& name,
  const glm::vec3& amb,
  const glm::vec3& diff,
  const glm::vec3& spec,
  float energy
)
  : m_name(name),
    m_ambient("ambient", amb),
    m_diffuse("diffuse", diff),
    m_specular("specular", spec),
    m_energy("energy", energy)
{}

GLTools::GLLight::~GLLight()
{}

void GLTools::GLLight::setShaderUniform(const GLShaderProgram& program) const
{
  program.setUniformValue(this->m_ambient_str, this->m_ambient.second);
  program.setUniformValue(this->m_diffuse_str, this->m_diffuse.second);
  program.setUniformValue(this->m_specular_str, this->m_specular.second);

  program.setUniformValue(this->m_energy_str, this->m_energy.second);
}

const glm::vec3& GLTools::GLLight::getAmbient() const
{
  return this->m_ambient.second;
}

const glm::vec3& GLTools::GLLight::getDiffuse() const
{
  return this->m_diffuse.second;
}

const glm::vec3& GLTools::GLLight::getSpecular() const
{
  return this->m_specular.second;
}

const float& GLTools::GLLight::getEnergy() const
{
  return this->m_energy.second;
}

const std::string& GLTools::GLLight::getShaderName() const
{
  return this->m_name;
}

void GLTools::GLLight::setShaderName(const std::string& name)
{
  this->m_name = name;
  this->buildShaderStrings();
}

void GLTools::GLLight::setAmbient(const glm::vec3& vec)
{
  this->m_ambient.second = vec;
}

void GLTools::GLLight::setDiffuse(const glm::vec3& vec)
{
  this->m_diffuse.second = vec;
}

void GLTools::GLLight::setSpecular(const glm::vec3& vec)
{
  this->m_specular.second = vec;
}

void GLTools::GLLight::setEnergy(float value)
{
  this->m_energy.second = value;
}

void GLTools::GLLight::init()
{
  this->buildShaderStrings();
}

void GLTools::GLLight::buildShaderStrings()
{
  this->m_ambient_str = this->m_name + '.' + this->m_ambient.first;
  this->m_diffuse_str = this->m_name + '.' + this->m_diffuse.first;
  this->m_specular_str = this->m_name + '.' + this->m_specular.first;

  this->m_energy_str = this->m_name + '.' + this->m_energy.first;
}
