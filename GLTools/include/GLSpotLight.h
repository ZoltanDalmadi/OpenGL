#pragma once

#include "GLLight.h"

namespace GLTools
{

class GLSpotLight : public GLLight
{
public:
  explicit
  GLSpotLight(const glm::vec3& pos = glm::vec3(0.0f),
              const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f),
              const glm::vec3& amb = glm::vec3(0.2f),
              const glm::vec3& diff = glm::vec3(0.8f),
              const glm::vec3& spec = glm::vec3(1.0f),
              float constant = 1.0f, float linear = 0.09f,
              float quad = 0.032f, float cutoff = glm::cos(glm::radians(10.0f)),
              float outerCutoff = glm::cos(glm::radians(20.0f)), float energy = 1.0f);

  virtual ~GLSpotLight();

  void setTarget(const glm::vec3& target);
  virtual void setShaderUniform(const GLShaderProgram& program) const override;

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& vec);

  const glm::vec3& getDirection() const;
  void setDirection(const glm::vec3& vec);

  const float& getConstant() const;
  void setConstant(const float& value);

  const float& getLinear() const;
  void setLinear(const float& value);

  const float& getQuadratic() const;
  void setQuadratic(const float& value);

  const float& getCutoff() const;
  void setCutoff(const float& value);

  const float& getOutercutoff() const;
  void setOutercutoff(const float& value);

protected:
  virtual void buildShaderStrings() override;

  std::pair<std::string, glm::vec3> m_position;
  std::pair<std::string, glm::vec3> m_direction;

  std::pair<std::string, float> m_constant;
  std::pair<std::string, float> m_linear;
  std::pair<std::string, float> m_quadratic;

  std::pair<std::string, float> m_cutoff;
  std::pair<std::string, float> m_outercutoff;

  std::string m_position_str;
  std::string m_direction_str;

  std::string m_constant_str;
  std::string m_linear_str;
  std::string m_quadratic_str;

  std::string m_cutoff_str;
  std::string m_outercutoff_str;
};

}
