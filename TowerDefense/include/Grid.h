#pragma once

#include "GLShaderProgram.h"
#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"

class Grid
{
public:
  Grid();
  Grid(float width, float height, float squareSize);
  ~Grid();

  void draw(const GLTools::GLShaderProgram& shaderProgram);
  void initialize();
  glm::vec3 getCenter(const glm::vec3& position);
  bool getCenter(const glm::vec3& position, glm::vec3& out);

  glm::vec3 color;
  float alpha;

private:
  float m_width;
  float m_height;
  float m_small_square;
  size_t m_line_size;
  std::vector<std::pair<glm::vec2, glm::vec2>> m_grid;

  GLTools::GLVertexArrayObject m_VAO;
  GLTools::GLBufferObject m_VBO;
  void uploadGrid();
};

