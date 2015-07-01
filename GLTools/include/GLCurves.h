#pragma once
#include <array>
#include <vector>
#include <glm\glm.hpp>
#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"

namespace GLTools
{

class GLCurves
{
  using iter2 = std::array<glm::vec3, 2>::iterator;
  using iter4 = std::array<glm::vec3, 4>::iterator;
public:
  GLCurves();
  GLCurves(GLCurves&, std::array<glm::vec3, 2>&);
  explicit GLCurves(const std::array<glm::vec3, 4>&);
  virtual ~GLCurves();

  void setControlPoints(const std::array<glm::vec3, 4>&);
  std::array<glm::vec3, 4>& getControlPoints();

  void initialize();
  void draw();
  void drawPoints();
  void update();

  glm::vec3 evaluateBezierPosition(float);
  glm::vec3 evaluateBezierTangent(float);
  std::pair<glm::vec3, glm::vec3> getPositionAndTangent(float t);

private:
  std::array<glm::vec3, 4> m_controlPoints;
  std::vector<glm::vec3> curvePoints;
  int detail;

  GLVertexArrayObject m_VAO;
  GLBufferObject m_VBO;
};

}
