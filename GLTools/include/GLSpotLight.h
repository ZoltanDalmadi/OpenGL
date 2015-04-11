#pragma once

#include <glm/glm.hpp>
#include <string>
#include <utility>
#include "GLShaderProgram.h"

namespace GLTools
{

class GLSpotLight
{
public:
  std::string m_name;

  std::pair<std::string, glm::vec3> m_position;
  std::pair<std::string, glm::vec3> m_direction;

  std::pair<std::string, glm::vec3> m_ambient;
  std::pair<std::string, glm::vec3> m_diffuse;
  std::pair<std::string, glm::vec3> m_specular;

  std::pair<std::string, float> m_constant;
  std::pair<std::string, float> m_linear;
  std::pair<std::string, float> m_quadratic;

  std::pair<std::string, float> m_cutoff;
  std::pair<std::string, float> m_outercutoff;

  std::pair<std::string, float> m_energy;

  GLSpotLight(const glm::vec3& pos = glm::vec3(0.0f),
              const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f),
              const glm::vec3& amb = glm::vec3(0.2f),
              const glm::vec3& diff = glm::vec3(0.8f),
              const glm::vec3& spec = glm::vec3(1.0f),
              float constant = 1.0f, float linear = 0.09f,
              float quad = 0.032f, float cutoff = glm::cos(glm::radians(10.0f)),
              float outerCutoff = glm::cos(glm::radians(20.0f)), float energy = 1.0f);

  virtual ~GLSpotLight();

  void setShaderUniform(const GLTools::GLShaderProgram& program) const;
  void buildShaderStrings();

  void setTarget(const glm::vec3& target);

  void setShaderName(const std::string& name);

private:
  std::string m_position_str;
  std::string m_direction_str;
  std::string m_ambient_str;
  std::string m_diffuse_str;
  std::string m_specular_str;

  std::string m_constant_str;
  std::string m_linear_str;
  std::string m_quadratic_str;

  std::string m_cutoff_str;
  std::string m_outercutoff_str;

  std::string m_energy_str;
};

}
