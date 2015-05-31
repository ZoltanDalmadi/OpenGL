#pragma once

#include "GLLight.h"

namespace GLTools
{

class GLPointLight : public GLLight
{
public:
  explicit
  GLPointLight(const glm::vec3& pos = glm::vec3(0.0f),
               const glm::vec3& amb = glm::vec3(0.2f),
               const glm::vec3& diff = glm::vec3(0.8f),
               const glm::vec3& spec = glm::vec3(1.0f),
               float constant = 1.0f, float linear = 0.09f,
               float quad = 0.032f, float energy = 1.0f);

  virtual ~GLPointLight();
  virtual void setShaderUniform(const GLShaderProgram& program) const override;

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& vec);

  const float& getConstant() const;
  void setConstant(const float& value);

  const float& getLinear() const;
  void setLinear(const float& value);

  const float& getQuadratic() const;
  void setQuadratic(const float& value);

protected:
  virtual void buildShaderStrings() override;

  std::pair<std::string, glm::vec3> m_position;

  std::pair<std::string, float> m_constant;
  std::pair<std::string, float> m_linear;
  std::pair<std::string, float> m_quadratic;

  std::string m_position_str;

  std::string m_constant_str;
  std::string m_linear_str;
  std::string m_quadratic_str;
};

}
