#pragma once

#include <utility>
#include "GLShaderProgram.h"

namespace GLTools
{

class GLLight
{
public:
  explicit GLLight(const std::string& name = "light",
                   const glm::vec3& amb = glm::vec3(0.2f),
                   const glm::vec3& diff = glm::vec3(0.8f),
                   const glm::vec3& spec = glm::vec3(1.0f),
                   float energy = 1.0f);
  virtual ~GLLight();

  virtual void setShaderUniform(const GLShaderProgram& program) const;

  const std::string& getShaderName() const;
  void setShaderName(const std::string& name);

  const glm::vec3& getAmbient() const;
  const glm::vec3& getDiffuse() const;
  const glm::vec3& getSpecular() const;
  const float& getEnergy() const;

  void setAmbient(const glm::vec3& vec);
  void setDiffuse(const glm::vec3& vec);
  void setSpecular(const glm::vec3& vec);
  void setEnergy(float value);

protected:
  void init();
  virtual void buildShaderStrings();

  std::string m_name;

  std::pair<std::string, glm::vec3> m_ambient;
  std::pair<std::string, glm::vec3> m_diffuse;
  std::pair<std::string, glm::vec3> m_specular;

  std::pair<std::string, float> m_energy;

  std::string m_ambient_str;
  std::string m_diffuse_str;
  std::string m_specular_str;

  std::string m_energy_str;
};

}
