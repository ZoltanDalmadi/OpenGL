#pragma once
#include <GLModel.h>

class Tower
{
public:
  explicit Tower(GLTools::GLModel *base = nullptr);
  virtual ~Tower();

  void setBaseModel(GLTools::GLModel *gl_model);

  void setTarget(glm::vec3 *target);
  void clearTarget();
  void draw(const GLTools::GLShaderProgram& shaderProgram);
  void update();

private:
  glm::vec3 m_position;
  glm::vec3 *m_target;
  GLTools::GLModel *m_base;
  //GLTools::GLModel cannon;

  glm::mat4 m_modelMatrix;
  glm::mat3 m_normalMatrix;
};
