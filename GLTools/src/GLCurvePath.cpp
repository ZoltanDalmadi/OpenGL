#include "GLCurvePath.h"

GLTools::GLCurvePath::GLCurvePath()
{}

GLTools::GLCurvePath::~GLCurvePath()
{}

std::pair<glm::vec3, glm::vec3> GLTools::GLCurvePath::getPositionAndTangent
(float t)
{
  std::pair<glm::vec3, glm::vec3> ret;

  auto size = m_data.size();
  auto oneSlashSize = 1.0f / size;

  for (auto i = 1; i <= size; i++)
  {
    if (t <= i * oneSlashSize)
    {
      auto vt = (t - (i - 1) * oneSlashSize) * size;

      ret = m_data[i - 1].getPositionAndTangent(vt);
      break;
    }
  }

  return ret;
}

void GLTools::GLCurvePath::draw()
{
  for (auto& curve : m_data)
    curve.draw();
}
