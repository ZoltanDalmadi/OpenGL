#include "GLCurvePath.h"


GLTools::GLCurvePath::GLCurvePath()
{
}


GLTools::GLCurvePath::~GLCurvePath()
{
}

std::pair<glm::vec3, glm::vec3> GLTools::GLCurvePath::getPositionAndTangent(
  float t)
{
  std::pair<glm::vec3, glm::vec3> ret;

  auto size = m_data.size();
  auto oneSlashSize = 1.0f / size;

  for (int i = 1; i <= size; i++)
  {
    if (t < i * oneSlashSize)
    {
      auto vt = 1 - (t * ((i - 1) * oneSlashSize)) + (t * (i *
                                                      oneSlashSize)) ;

      return m_data[i - 1].getPositionAndTangent(vt);
    }
  }

  return ret;
}

void GLTools::GLCurvePath::draw()
{
  for (auto i : m_data)
  {
    i.render();
  }
}
