#include "GLCurves.h"

GLTools::GLCurves::GLCurves()
{}

GLTools::GLCurves::~GLCurves()
{}

/*
C1 continous connection.
*/
GLTools::GLCurves::GLCurves
(GLCurves& firstCurve, std::array<glm::vec3, 2>& controlPoints)
{
  auto midPoint = (firstCurve.m_controlPoints[2] +
                   firstCurve.m_controlPoints[3]) / 2.0f;

  m_controlPoints[1] = firstCurve.m_controlPoints[3];
  firstCurve.m_controlPoints[3] = midPoint;

  m_controlPoints[0] = midPoint;
  m_controlPoints[2] = controlPoints[0];
  m_controlPoints[3] = controlPoints[1];

  detail = 1000;

  firstCurve.update();
  update();
}

GLTools::GLCurves::GLCurves(const std::array<glm::vec3, 4>& controlPoints)
  : m_controlPoints(controlPoints), detail(1000)
{}

void GLTools::GLCurves::setControlPoints
(const std::array<glm::vec3, 4>& controlPoints)
{
  m_controlPoints = controlPoints;
}

std::array<glm::vec3, 4>& GLTools::GLCurves::getControlPoints()
{
  return m_controlPoints;
}

void GLTools::GLCurves::initialize()
{
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(curvePoints.data(),
               curvePoints.size() * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 3, sizeof(glm::vec3));
  m_VAO.unbind();
}

void GLTools::GLCurves::draw()
{
  m_VAO.bind();
  glDrawArrays(GL_POINTS, 0, curvePoints.size());
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
  auto OneMinusT = 1.0f - t;

  auto b0 = OneMinusT * OneMinusT * OneMinusT;
  auto b1 = 3.0f * t * OneMinusT * OneMinusT;
  auto b2 = 3.0f * t * t * OneMinusT;
  auto b3 = t * t * t;

  return b0 * m_controlPoints[0] + b1 * m_controlPoints[1] +
         b2 * m_controlPoints[2] + b3 * m_controlPoints[3];
}

glm::vec3 GLTools::GLCurves::evaluateBezierTangent(float t)
{
  auto OneMinusT = 1.0f - t;

  auto b0 = 3.0f * OneMinusT * OneMinusT;
  auto b1 = 6.0f * t * OneMinusT;
  auto b2 = 3.0f * t * t;

  return b0 * (m_controlPoints[1] - m_controlPoints[0]) +
         b1 * (m_controlPoints[2] - m_controlPoints[1]) +
         b2 * (m_controlPoints[3] - m_controlPoints[2]);
}

std::pair<glm::vec3, glm::vec3> GLTools::GLCurves::getPositionAndTangent
(float t)
{
  return std::make_pair(evaluateBezierPosition(t), evaluateBezierTangent(t));
}


void GLTools::GLCurves::update()
{
  float OneOverDetail = 1.0 / float(detail - 1.0);

  for (int i = 0; i < detail; i++)
  {
    float t = i * OneOverDetail;
    curvePoints.emplace_back(evaluateBezierPosition(t));
  }
}