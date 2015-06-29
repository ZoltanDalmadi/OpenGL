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

void GridPlane::initialize()
{
  m_plane->initialize();
  uploadGrid();
}

void GridPlane::uploadGrid()
{
  auto pieceX = m_width / 2;
  auto pieceY = m_height / 2;

  for (float i = -pieceX; i < pieceX; i += m_small_square)
  {
    for (float j = -pieceY; j < pieceY; j += m_small_square)
    {
      std::cout << i << " " << j << std::endl;
    }
  }
}

GridPlane::~GridPlane()
{
}
