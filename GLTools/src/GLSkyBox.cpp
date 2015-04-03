#include "GLSkyBox.h"

GLTools::GLSkyBox::GLSkyBox(float size)
{
  float side2 = size / 2.0f;
  SimpleVertex vert;

  // Front
  vert.position = glm::vec3(-side2, -side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, side2);
  m_vertices.push_back(vert);

  // Right
  vert.position = glm::vec3(side2, -side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  m_vertices.push_back(vert);

  // Back
  vert.position = glm::vec3(-side2, -side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  m_vertices.push_back(vert);

  // Left
  vert.position = glm::vec3(-side2, -side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, -side2, -side2);
  m_vertices.push_back(vert);

  // Bottom
  vert.position = glm::vec3(-side2, -side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, -side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, -side2, side2);
  m_vertices.push_back(vert);

  // Top
  vert.position = glm::vec3(-side2, side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(side2, side2, -side2);
  m_vertices.push_back(vert);

  vert.position = glm::vec3(-side2, side2, -side2);
  m_vertices.push_back(vert);

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

GLTools::GLSkyBox::~GLSkyBox()
{}

void GLTools::GLSkyBox::initialize()
{
  m_VAO.create();
  m_VBO.create();
  m_EBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_vertices.data(), m_vertices.size() * sizeof(SimpleVertex));
  m_EBO.bind();
  m_EBO.upload(m_indices.data(), m_indices.size() * sizeof(GLuint));

  m_VAO.setAttributeArray(0, 3, sizeof(SimpleVertex));

  m_VAO.unbind();
}
