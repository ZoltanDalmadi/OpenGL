#pragma once
#include <GLModel.h>

class Tower
{
public:
  explicit Tower(GLTools::GLModel *base, GLTools::GLModel *cannon);
  virtual ~Tower();

  const glm::vec3& getPosition() const;
  void setPosition(const glm::vec3& pos);

  const glm::vec3& getOffset() const;
  void setOffset(const glm::vec3& offset);

  const float& getBaseAngle() const;
  const float& getCannonAngle() const;

  void setTarget(glm::vec3 *target);
  void clearTarget();
  void draw(const GLTools::GLShaderProgram& shaderProgram);
  void update();

private:
  GLTools::GLModel *m_base;
  GLTools::GLModel *m_cannon;
  float m_baseAngle;
  float m_cannonAngle;

  glm::vec3 m_position;
  glm::vec3 *m_target;
  glm::vec3 m_offset = glm::vec3(0.33183f, 0.93760f, 0.0f);

  glm::mat4 m_modelMatrix;
  glm::mat4 m_cannonMatrix;
};
