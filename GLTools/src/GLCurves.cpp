#include "GLCurves.h"


GLTools::GLCurves::GLCurves()
{
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_controlPoints.data(),
               m_controlPoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 4, sizeof(glm::vec3));

  m_VAO.unbind();
}

/*
C1 incessant.
*/
GLTools::GLCurves::GLCurves(GLCurves firstCurve,
                            std::array<glm::vec3, 2> controlPoints)
{
  m_controlPoints[0] = firstCurve.getControlPoints().at(3);
  glm::vec3 fourMinusThree = firstCurve.getControlPoints().at(3)
                             - firstCurve.getControlPoints().at(3);

  m_controlPoints[1] = m_controlPoints[0] + fourMinusThree;
  m_controlPoints[2] = controlPoints[0];
  m_controlPoints[3] = controlPoints[1];

  GLCurves();
}

GLTools::GLCurves::GLCurves(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }

  //GLCurves();
}

void GLTools::GLCurves::setControlPoints(std::array<glm::vec3, 4> controlPoints)
{
  for (int i = 0; i < controlPoints.size(); i++)
  {
    m_controlPoints[i] = controlPoints[i];
  }

  GLCurves();
}

std::array<glm::vec3, 4> GLTools::GLCurves::getControlPoints()
{
  return m_controlPoints;
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
  /*glDrawElements(GL_POINTS, 12,
                 GL_UNSIGNED_INT, nullptr);*/

  m_VAO.unbind();
}

glm::vec3 GLTools::GLCurves::evaluateBezierPosition(float t)
{
  float OneMinusT = 1.0 - t;
  float b0 = OneMinusT * OneMinusT * OneMinusT;
  float b1 = 3.0 * t * OneMinusT * OneMinusT;
  float b2 = 3.0 * t * t * OneMinusT;
  float b3 = t * t * t;
  return b0 * m_controlPoints.at(0) + b1 * m_controlPoints.at(
           1) + b2 * m_controlPoints.at(2) + b3 * m_controlPoints.at(3);
}

glm::vec3 GLTools::GLCurves::evaluateBezierTangent(float t)
{
  return glm::vec3(1.0f);
}

GLTools::GLCurves::~GLCurves()
{
}
