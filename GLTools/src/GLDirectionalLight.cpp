#include "GLDirectionalLight.h"

GLTools::GLDirectionalLight::GLDirectionalLight
(const glm::vec3& dir, const glm::vec3& amb,
 const glm::vec3& diff, const glm::vec3& spec, float energy)
  : m_name("dirLight"), m_direction("direction", dir),
    m_ambient("ambient", amb),
    m_diffuse("duffuse", diff),
    m_specular("specular", spec),
    m_energy("energy", energy)
{
  buildShaderStrings();
}

GLTools::GLDirectionalLight::~GLDirectionalLight()
{}

void GLTools::GLDirectionalLight::setShaderUniform
(const GLTools::GLShaderProgram& program)
{
  program.setUniformValue(m_direction_str, m_direction.second);

  program.setUniformValue(m_ambient_str, m_ambient.second);
  program.setUniformValue(m_diffuse_str, m_diffuse.second);
  program.setUniformValue(m_specular_str, m_specular.second);

  program.setUniformValue(m_energy_str, m_energy.second);
}

void GLTools::GLDirectionalLight::buildShaderStrings()
{
  m_direction_str = m_name + '.' + m_direction.first;

  m_ambient_str = m_name + '.' + m_ambient.first;
  m_diffuse_str = m_name + '.' + m_diffuse.first;
  m_specular_str = m_name + '.' + m_specular.first;

  m_energy_str = m_name + '.' + m_energy.first;
}