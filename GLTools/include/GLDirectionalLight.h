#pragma once

#include "GLLight.h"

namespace GLTools
{

class GLDirectionalLight : public GLLight
{
public:
  GLDirectionalLight(const glm::vec3& dir,
                     const glm::vec3& amb,
                     const glm::vec3& diff,
                     const glm::vec3& spec,
                     float energy);

  virtual ~GLDirectionalLight();

  const glm::vec3& getDirection() const;
  void setDirection(const glm::vec3& vec);
  virtual void setShaderUniform(const GLShaderProgram& program) const override;

protected:
  virtual void buildShaderStrings() override;
  std::pair<std::string, glm::vec3> m_direction;
  std::string m_direction_str;
};

}
