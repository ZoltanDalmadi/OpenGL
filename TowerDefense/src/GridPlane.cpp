#include "GridPlane.h"
#include <iostream>



GridPlane::GridPlane()
{
}

GridPlane::GridPlane(float width, float height, float squareSize)
{
  m_width = width;
  m_height = height;
  m_plane = std::make_unique<GLTools::GLPlane>(m_width, m_height);
  m_small_square = squareSize;
}

void GridPlane::draw()
{
  m_plane->draw();
}

void GridPlane::drawLines()
{
  m_VAO.bind();
  glDrawArrays(GL_LINES, 0, m_line_size);
  m_VAO.unbind();
}

void GridPlane::initialize()
{
  m_plane->initialize();
  uploadGrid();

  std::vector<glm::vec3> linePoints;

  auto pieceX = m_width / 2;
  auto pieceY = m_height / 2;

  for (float i = -pieceX; i <= pieceX; i += m_small_square)
  {
    glm::vec3 left(i, -pieceY, 0.0f);
    linePoints.emplace_back(left);
    glm::vec3 upRight(i, pieceY, 0.0f);
    linePoints.emplace_back(upRight);
  }

  for (float i = -pieceY; i <= pieceY; i += m_small_square)
  {
    glm::vec3 left(-pieceX, i, 0.0f);
    linePoints.emplace_back(left);
    glm::vec3 upRight(pieceX, i, 0.0f);
    linePoints.emplace_back(upRight);
  }

  m_line_size = linePoints.size();

  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(linePoints.data(),
               linePoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 3, sizeof(glm::vec3));
  m_VAO.unbind();
}

void GridPlane::uploadGrid()
{
  auto pieceX = m_width / 2;
  auto pieceY = m_height / 2;

  for (float i = -pieceX; i < pieceX; i += m_small_square)
  {
    for (float j = -pieceY; j < pieceY; j += m_small_square)
    {
      glm::vec2 left(i, j);
      glm::vec2 upRight(i + m_small_square, j + m_small_square);

      std::pair<glm::vec2, glm::vec2> temp(left, upRight);

      m_grid.emplace_back(temp);
    }
  }
}

bool GridPlane::getCenter(const glm::vec3& position, glm::vec3& out)
{
  bool ret = false;
  out = position;

  for (auto& i : m_grid)
  {
    if (i.first.x <= position.x && i.second.x >= position.x)
    {
      if (i.first.y <= position.z && i.second.y >= position.z)
      {
        glm::vec2 tangent = (i.second - i.first) / 2.0f;
        glm::vec2 center = i.first + tangent;
        ret = true;
        out = glm::vec3(center.x, position.y, center.y);
        break;
      }
    }
  }

  return ret;
}

void GridPlane::setMaterial(GLTools::GLMaterial *material)
{
  m_plane->m_material = material;
}

GridPlane::~GridPlane()
{
}
