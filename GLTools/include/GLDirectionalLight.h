#pragma once

#include <glm/glm.hpp>
#include <string>
#include <utility>
#include "GLShaderProgram.h"

namespace GLTools
{

class GLDirectionalLight
{
public:
  std::string m_name;

  std::pair<std::string, glm::vec3> m_direction;

  std::pair<std::string, glm::vec3> m_ambient;
  std::pair<std::string, glm::vec3> m_diffuse;
  std::pair<std::string, glm::vec3> m_specular;

  std::pair<std::string, float> m_energy;

  GLDirectionalLight(const glm::vec3& dir,
                     const glm::vec3& amb,
                     const glm::vec3& diff,
                     const glm::vec3& spec,
                     float energy);

  virtual ~GLDirectionalLight();

  void setShaderUniform(const GLTools::GLShaderProgram& program);
  void buildShaderStrings();

private:
  std::string m_direction_str;

  std::string m_ambient_str;
  std::string m_diffuse_str;
  std::string m_specular_str;

  std::string m_energy_str;
};

}
