#include "GLCurves.h"

using namespace GLTools;

GLCurves::GLCurves()
{
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_controlPoints.data(),
               m_controlPoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 4, sizeof(float));

  m_VAO.unbind();
}

/*
C1 incessant.
*/
GLCurves::GLCurves(GLCurves firstCurve,
                   std::array<glm::vec3, 2> controlPoints)
{
  m_controlPoints[0] = firstCurve.getControlPoints().at(3);
  glm::vec3 fourMinusThree = firstCurve.getControlPoints().at(3)
                             - firstCurve.getControlPoints().at(3);

  m_controlPoints[1] = m_controlPoints[0] + fourMinusThree;
  m_controlPoints[2] = controlPoints[0];
  m_controlPoints[3] = controlPoints[1];
}

GLCurves::GLCurves(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }
}

void GLCurves::setControlPoints(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }
}

std::array<glm::vec3, 4> GLCurves::getControlPoints()
{
  return m_controlPoints;
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
  return glm::vec3(1.0f);
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
  glDrawElements(GL_POINTS, static_cast<GLsizei>(m_controlPoints.size()),
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
  return b0 * m_controlPoints.at(0) + b1 * m_controlPoints.at(
           1) + b2 * m_controlPoints.at(2) + b3 * m_controlPoints.at(3);
}

glm::vec3 GLCurves::evaluateBezierTangent(float t)
{
  return glm::vec3(1.0f);
}

GLCurves::~GLCurves()
{
}
