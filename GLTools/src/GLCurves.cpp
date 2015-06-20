#include "GLCurves.h"

using namespace GLTools;

GLCurves::GLCurves()
{
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(controlPoints.data(), controlPoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 4, sizeof(float));

  m_VAO.unbind();
}

/*
C1 incessant.
*/
GLCurves::GLCurves(const GLCurves firstCurve,
                   std::array<glm::vec3, 2> controlPoints)
{
  this->controlPoints[0] = firstCurve.getControlPoints.at(3);
  glm::vec3 fourMinusThree = firstCurve.getControlPoints.at(
                               3) - firstCurve.getControlPoints.a
                             (2);

  this->controlPoints[1] = this->controlPoints.at(0) + fourMinusThree;
  this->controlPoints[2] = controlPoints.at(0);
  this->controlPoints[3] = controlPoints.at(1);
}

GLCurves::GLCurves(std::array<glm::vec3, 4> controlPoints)
{
  std::copy(controlPoints.begin(), controlPoints.end(), this->controlPoints);
}

void GLCurves::setControlPoints(std::array<glm::vec3, 4> controlPoints)
{
  std::copy(controlPoints.begin(), controlPoints.end(), this->controlPoints);
}

std::array<glm::vec3, 4> GLCurves::getControlPoints()
{
  return this->controlPoints;
}

/*
Get position in T;
*/
glm::vec3 GLCurves::getPosition(double t)
{
  return evaluateBezierPosition(t);
}

/*
Get position with the class t.
*/
glm::vec3 GLCurves::getPosition()
{
  return getPosition(this->t);
}

/*
Return the tangent of the t.
*/
glm::vec3 GLCurves::getTangent(double t)
{
}

/*
Return the tangent of the class t.
*/
glm::vec3 GLCurves::getTangent()
{
  return getTangent(this->t);
}

void GLCurves::setT(double t)
{
  this->t = t;
}

double GLCurves::getT()
{
  return this->t;
}

void GLCurves::render()
{
  m_VAO.bind();
  glDrawElements(GL_POINTS, static_cast<GLsizei>(controlPoints.size()),
                 GL_UNSIGNED_INT, nullptr);

  m_VAO.unbind();
}

glm::vec3 GLCurves::evaluateBezierPosition(float t)
{
  float OneMinusT = 1.0 - t;
  float b0 = OneMinusT * OneMinusT * OneMinusT;
  float b1 = 3.0 * t * OneMinusT * OneMinusT;
  float b2 = 3.0 * t * t * OneMinusT;
  float b3 = t * t * t;
  return b0 * this->controlPoints.at(0) + b1 * this->controlPoints.at(
           1) + b2 * this->controlPoints.at(2) + b3 * this->controlPoints.at(3);
}

glm::vec3 GLCurves::evaluateBezierTangent(float t)
{
}

GLCurves::~GLCurves()
{
}
