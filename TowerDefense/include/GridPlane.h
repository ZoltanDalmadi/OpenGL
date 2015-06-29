#pragma once
#include <memory>

#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"
#include "GLPlane.h"

class GridPlane
{
public:
  GridPlane();
  GridPlane(float width, float height, float squareSize);
  ~GridPlane();

  void draw();
  void drawLines();
  void initialize();
  glm::vec3 GridPlane::getCenter(glm::vec3 position);

  void setMaterial(GLTools::GLMaterial *material);

private:
  std::unique_ptr<GLTools::GLPlane> m_plane;
  float m_width;
  float m_height;
  float m_small_square;
  int m_line_size;
  std::vector<std::pair<glm::vec2, glm::vec2>> m_grid;

  GLTools::GLVertexArrayObject m_VAO;
  GLTools::GLBufferObject m_VBO;
  void uploadGrid();
};

