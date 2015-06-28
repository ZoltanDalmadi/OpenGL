#pragma once
#include <array>
#include <glm\glm.hpp>
#include "GLVertexArrayObject.h"
#include "GLBufferObject.h"

namespace GLTools
{

class GLCurves
{
public:
  GLCurves();
  GLCurves(GLCurves&, std::array<glm::vec3, 2>);
  GLCurves(std::array<glm::vec3, 4>);
  void setControlPoints(std::array<glm::vec3, 4>);
  std::array<glm::vec3, 4> getControlPoints();

  void initialize();
  /*
  Get position in T;
  */
  glm::vec3 getPosition(double);

  /*
  Get position with the class t.
  */
  glm::vec3 getPosition();

  /*
  Return the tangent of the t.
  */
  glm::vec3 getTangent(double);

  /*
  Return the tangent of the class t.
  */
  glm::vec3 getTangent();

  void setT(double);
  double getT();
  void render();

  void drawPoints();

  std::pair<glm::vec3, glm::vec3> getPositionAndTangent(float t);

  virtual ~GLCurves();

private:
  double t;
  std::array<glm::vec3, 4> m_controlPoints;
  glm::vec3 evaluateBezierPosition(float);
  glm::vec3 evaluateBezierTangent(float);

  GLVertexArrayObject m_VAO;
  GLBufferObject m_VBO;
};

}
