#include "GLCurves.h"


GLTools::GLCurves::GLCurves()
{
}

/*
C1 incessant.
*/
GLTools::GLCurves::GLCurves(GLCurves& firstCurve,
                            std::array<glm::vec3, 2> controlPoints)
{
  glm::vec3 midPoint = (firstCurve.getControlPoints().at(2) +
                        firstCurve.getControlPoints().at(3)) / glm::vec3(2.0f);

  m_controlPoints[1] = firstCurve.getControlPoints().at(3);
  firstCurve.m_controlPoints[3] = midPoint;

  m_controlPoints[0] = midPoint;
  m_controlPoints[2] = controlPoints[0];
  m_controlPoints[3] = controlPoints[1];
}

GLTools::GLCurves::GLCurves(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }
}

void GLTools::GLCurves::setControlPoints(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }
}

std::array<glm::vec3, 4> GLTools::GLCurves::getControlPoints()
{
  return m_controlPoints;
}

void GLTools::GLCurves::initialize()
{
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_controlPoints.data(),
               m_controlPoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 3, sizeof(glm::vec3));
  m_VAO.unbind();
}

/*
Get position in T;
*/
glm::vec3 GLTools::GLCurves::getPosition(double t)
{
  return evaluateBezierPosition(t);
}

/*
Get position with the class t.
*/
glm::vec3 GLTools::GLCurves::getPosition()
{
  return getPosition(this->t);
}

/*
Return the tangent of the t.
*/
glm::vec3 GLTools::GLCurves::getTangent(double t)
{
  return glm::vec3(1.0f);
}

/*
Return the tangent of the class t.
*/
glm::vec3 GLTools::GLCurves::getTangent()
{
  return getTangent(this->t);
}

void GLTools::GLCurves::setT(double t)
{
  this->t = t;
}

double GLTools::GLCurves::getT()
{
  return this->t;
}

void GLTools::GLCurves::render()
{
  m_VAO.bind();
  glDrawArrays(GL_LINES_ADJACENCY, 0, m_controlPoints.size());
  m_VAO.unbind();
}

void GLTools::GLCurves::drawPoints()
{
  m_VAO.bind();
  glDrawArrays(GL_POINTS, 0, m_controlPoints.size());
  m_VAO.unbind();
}

glm::vec3 GLTools::GLCurves::evaluateBezierPosition(float t)
{
  float vt = 0.0f;

  if (t > 0.5)
    vt = t * 2;
  else
    vt = (t * 2) - 0.5;

  float OneMinusT = 1.0 - vt;

  float b0 = OneMinusT * OneMinusT * OneMinusT;
  float b1 = 3.0 * vt * OneMinusT * OneMinusT;
  float b2 = 3.0 * vt * vt * OneMinusT;
  float b3 = vt * vt * vt;
  return b0 * m_controlPoints.at(0) + b1 * m_controlPoints.at(
           1) + b2 * m_controlPoints.at(2) + b3 * m_controlPoints.at(3);
}

glm::vec3 GLTools::GLCurves::evaluateBezierTangent(float t)
{
  float vt = 0.0f;

  if (t > 0.5)
    vt = t * 2;
  else
    vt = (t * 2) - 0.5;

  float OneMinusT = 1.0 - vt;

  float b0 = 3 * OneMinusT * OneMinusT;
  float b1 = 6 * vt * OneMinusT;
  float b2 = 3 * vt * vt;

  return b0 * (m_controlPoints.at(1) - m_controlPoints.at(0)) +
         b1 * (m_controlPoints.at(2) - m_controlPoints.at(1)) +
         b2 * (m_controlPoints.at(3) - m_controlPoints.at(2));
}

std::pair<glm::vec3, glm::vec3> GLTools::GLCurves::getPositionAndTangent(
  float t)
{
  std::pair<glm::vec3, glm::vec3> ret;
  ret.first = evaluateBezierPosition(t);
  ret.second = evaluateBezierTangent(t);

  return ret;
}

GLTools::GLCurves::~GLCurves()
{
}
