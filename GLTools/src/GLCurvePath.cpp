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

  for (int i = 1; i <= m_data.size(); i++)
  {
    if (t < i * (1 / m_data.size()))
    {
      float vt = 0.0f;

      vt = 1 - (t * ((i - 1) * (1 / m_data.size()))) + (t * (i *
                                                        (1 / m_data.size()))) ;

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
