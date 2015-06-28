#pragma once
#include <vector>
#include "GLCurves.h"

namespace GLTools
{

class GLCurvePath
{
public:
  GLCurvePath();
  ~GLCurvePath();
  std::vector<GLCurves> m_data;

  std::pair<glm::vec3, glm::vec3> getPositionAndTangent(
    float t);

  void draw();
};

}
