#include "GLPlane.h"

GLTools::GLPlane::GLPlane(float width, float height)
{
  auto width2 = width / 2.0f;
  auto height2 = height / 2.0f;
  Vertex vert;

  vert.position = glm::vec3(-width2, -height2, 0);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(width2, -height2, 0);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(width2, height2, 0);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-width2, height2, 0);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  m_indices = { 0, 2, 1, 0, 3, 2 };
}

GLTools::GLPlane::~GLPlane()
{}
