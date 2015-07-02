#pragma once
#include <array>
#include "GLShaderProgram.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"

namespace GLTools
{

class GLBoundingBox
{
public:
  GLBoundingBox();
  virtual ~GLBoundingBox();

  void initialize();

  void draw(const GLShaderProgram& shaderProgram,
            const glm::vec3& center, const glm::vec3& size,
            const glm::mat4& modelMatrix);
  void draw();

  void update(const glm::vec3& center, const glm::vec3& size);

private:
  std::array<glm::vec3, 8> m_vertices;
  std::array<GLuint, 16> m_indices;

  glm::mat4 m_modelMatrix;

  GLVertexArrayObject m_VAO;
  GLBufferObject m_VBO;
  GLBufferObject m_EBO;
};

}
