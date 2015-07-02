#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "GLShaderProgram.h"
#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"

class Laser
{
public:
  Laser();
  void initialize();
  void draw(GLTools::GLShaderProgram& shaderProgram);

  void commitDataChange();
  ~Laser();

  glm::vec3 m_data[2];
private:

  GLTools::GLVertexArrayObject m_VAO;
  GLTools::GLBufferObject m_VBO;
};

