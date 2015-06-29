#pragma once
#include <memory>

#include "GLPlane.h"

class GridPlane
{
public:
  GridPlane();
  GridPlane(float width, float height, float squareSize);
  ~GridPlane();

  void draw();
  void initialize();

private:
  std::unique_ptr<GLTools::GLPlane> m_plane;
  float m_width;
  float m_height;
  float m_small_square;
  std::vector<std::pair<glm::vec2, glm::vec2>> m_grid;

  void uploadGrid();
};

