#include "Grid.h"

Grid::Grid()
{}

Grid::Grid(float width, float height, float squareSize)
  : m_width(width),
    m_height(height),
    m_small_square(squareSize)
{}

Grid::~Grid()
{}

void Grid::initialize()
{
  color = glm::vec3(0.0f);
  alpha = 1.0f;

  uploadGrid();

  std::vector<glm::vec3> linePoints;

  auto pieceX = m_width / 2;
  auto pieceY = m_height / 2;

  for (auto i = -pieceX; i <= pieceX; i += m_small_square)
  {
    glm::vec3 left(i, 0.0f, -pieceY);
    linePoints.emplace_back(left);
    glm::vec3 upRight(i, 0.0f, pieceY);
    linePoints.emplace_back(upRight);
  }

  for (auto i = -pieceY; i <= pieceY; i += m_small_square)
  {
    glm::vec3 left(-pieceX, 0.0f, i);
    linePoints.emplace_back(left);
    glm::vec3 upRight(pieceX, 0.0f, i);
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

void Grid::uploadGrid()
{
  auto pieceX = m_width / 2;
  auto pieceY = m_height / 2;

  for (auto i = -pieceX; i < pieceX; i += m_small_square)
  {
    for (auto j = -pieceY; j < pieceY; j += m_small_square)
    {
      glm::vec2 left(i, j);
      glm::vec2 upRight(i + m_small_square, j + m_small_square);

      std::pair<glm::vec2, glm::vec2> temp(left, upRight);

      m_grid.emplace_back(temp);
    }
  }
}

glm::vec3 Grid::getCenter(const glm::vec3& position)
{
  glm::vec3 result;
  getCenter(position, result);
  return result;
}

bool Grid::getCenter(const glm::vec3& position, glm::vec3& out)
{
  auto ret = false;
  out = position;

  for (auto& i : m_grid)
  {
    if (i.first.x <= position.x && i.second.x >= position.x)
    {
      if (i.first.y <= position.z && i.second.y >= position.z)
      {
        auto tangent = (i.second - i.first) / 2.0f;
        auto center = i.first + tangent;
        ret = true;
        out = glm::vec3(center.x, position.y, center.y);
        break;
      }
    }
  }

  return ret;
}

void Grid::draw(const GLTools::GLShaderProgram& shaderProgram)
{
  shaderProgram.setUniformValue("gridColor", glm::vec4(color, alpha));
  m_VAO.bind();
  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_line_size));
  m_VAO.unbind();
}
