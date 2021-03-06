#include "GLCube.h"

GLTools::GLCube::GLCube(float size, bool insideOut)
{
  float side2 = size / 2.0f;
  Vertex vert;

  // Front
  vert.position = glm::vec3(-side2, -side2, side2);
  vert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, side2);
  vert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  vert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, side2);
  vert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  // Right
  vert.position = glm::vec3(side2, -side2, side2);
  vert.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  vert.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  vert.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  vert.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  // Back
  vert.position = glm::vec3(-side2, -side2, -side2);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  vert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  // Left
  vert.position = glm::vec3(-side2, -side2, side2);
  vert.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, side2);
  vert.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  vert.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, -side2, -side2);
  vert.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  // Bottom
  vert.position = glm::vec3(-side2, -side2, side2);
  vert.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, -side2, -side2);
  vert.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  vert.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, side2);
  vert.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  // Top
  vert.position = glm::vec3(-side2, side2, side2);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 0.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(1.0f, 1.0f);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  vert.texCoords = glm::vec2(0.0f, 1.0f);
  m_vertices.push_back(vert);

  if (insideOut)
  {
    for (auto& vertex : m_vertices)
      vertex.normal *= -1;

    m_indices =
    {
      0, 2, 1, 0, 3, 2,
      4, 6, 5, 4, 7, 6,
      8, 10, 9, 8, 11, 10,
      12, 14, 13, 12, 15, 14,
      16, 18, 17, 16, 19, 18,
      20, 22, 21, 20, 23, 22
    };
  }
  else
  {
    m_indices =
    {
      0, 1, 2, 0, 2, 3,
      4, 5, 6, 4, 6, 7,
      8, 9, 10, 8, 10, 11,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 21, 22, 20, 22, 23
    };
  }
}

GLTools::GLCube::~GLCube()
{}
