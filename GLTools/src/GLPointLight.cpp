#include "GLPointLight.h"

GLTools::GLPointLight::GLPointLight
(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff,
 const glm::vec3& spec, float constant, float linear, float quad, float energy)
  : m_name("pointLight"), m_position("position", pos), m_ambient("ambient", amb),
    m_diffuse("diffuse", diff), m_specular("specular", spec),
    m_constant("constant", constant), m_linear("linear", linear),
    m_quadratic("quadratic", quad), m_energy("energy", energy)
{
  buildShaderStrings();
}

GLTools::GLPointLight::~GLPointLight()
{}

void GLTools::GLPointLight::setShaderUniform
(const GLTools::GLShaderProgram& program)
{
  program.setUniformValue(m_position_str.c_str(), m_position.second);

  program.setUniformValue(m_ambient_str.c_str(), m_ambient.second);
  program.setUniformValue(m_diffuse_str.c_str(), m_diffuse.second);
  program.setUniformValue(m_specular_str.c_str(), m_specular.second);

  program.setUniformValue(m_constant_str.c_str(), m_constant.second);
  program.setUniformValue(m_linear_str.c_str(), m_linear.second);
  program.setUniformValue(m_quadratic_str.c_str(), m_quadratic.second);

  program.setUniformValue(m_energy_str.c_str(), m_energy.second);
}

void GLTools::GLPointLight::buildShaderStrings()
{
  m_position_str = m_name + '.' + m_position.first;

  m_ambient_str = m_name + '.' + m_ambient.first;
  m_diffuse_str = m_name + '.' + m_diffuse.first;
  m_specular_str = m_name + '.' + m_specular.first;

  m_constant_str = m_name + '.' + m_constant.first;
  m_linear_str = m_name + '.' + m_linear.first;
  m_quadratic_str = m_name + '.' + m_quadratic.first;
  m_energy_str = m_name + '.' + m_energy.first;
}
