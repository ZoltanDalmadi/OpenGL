#include "GLSpotLight.h"

GLTools::GLSpotLight::GLSpotLight
(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& amb,
 const glm::vec3& diff, const glm::vec3& spec,
 float constant, float linear, float quad, float cutoff,
 float outerCutoff, float energy)
  : m_name("spotLight"), m_position("position", pos),
    m_direction("direction", dir), m_ambient("ambient", amb),
    m_diffuse("diffuse", diff), m_specular("specular", spec),
    m_constant("constant", constant), m_linear("linear", linear),
    m_quadratic("quadratic", quad), m_cutoff("cutOff", cutoff),
    m_outercutoff("outerCutOff", outerCutoff), m_energy("energy", energy)
{
  buildShaderStrings();
}

GLTools::GLSpotLight::~GLSpotLight()
{}

void GLTools::GLSpotLight::setShaderUniform
(const GLTools::GLShaderProgram& program)
{
  program.setUniformValue(m_position_str.c_str(), m_position.second);
  program.setUniformValue(m_direction_str.c_str(), m_direction.second);

  program.setUniformValue(m_ambient_str.c_str(), m_ambient.second);
  program.setUniformValue(m_diffuse_str.c_str(), m_diffuse.second);
  program.setUniformValue(m_specular_str.c_str(), m_specular.second);

  program.setUniformValue(m_constant_str.c_str(), m_constant.second);
  program.setUniformValue(m_linear_str.c_str(), m_linear.second);
  program.setUniformValue(m_quadratic_str.c_str(), m_quadratic.second);

  program.setUniformValue(m_cutoff_str.c_str(), m_cutoff.second);
  program.setUniformValue(m_outercutoff_str.c_str(), m_outercutoff.second);

  program.setUniformValue(m_energy_str.c_str(), m_energy.second);
}

void GLTools::GLSpotLight::buildShaderStrings()
{
  m_position_str = m_name + '.' + m_position.first;
  m_direction_str = m_name + '.' + m_direction.first;

  m_ambient_str = m_name + '.' + m_ambient.first;
  m_diffuse_str = m_name + '.' + m_diffuse.first;
  m_specular_str = m_name + '.' + m_specular.first;

  m_constant_str = m_name + '.' + m_constant.first;
  m_linear_str = m_name + '.' + m_linear.first;
  m_quadratic_str = m_name + '.' + m_quadratic.first;

  m_cutoff_str = m_name + '.' + m_cutoff.first;
  m_outercutoff_str = m_name + '.' + m_outercutoff.first;

  m_energy_str = m_name + '.' + m_energy.first;
}
