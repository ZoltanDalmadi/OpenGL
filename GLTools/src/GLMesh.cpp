#include "GLMesh.h"

GLTools::GLMesh::GLMesh()
{
  m_EBO.setType(GLTools::GLBufferObject::BufferType::IndexBuffer);
}

GLTools::GLMesh::GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i)
  : m_vertices(std::move(v)),
    m_indices(std::move(i))
{
  m_EBO.setType(GLTools::GLBufferObject::BufferType::IndexBuffer);
  initialize();
}

GLTools::GLMesh::~GLMesh()
{}

void GLTools::GLMesh::initialize()
{
  m_VAO.create();
  m_VBO.create();
  m_EBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
  m_EBO.bind();
  m_EBO.upload(m_indices.data(), m_indices.size() * sizeof(GLuint));

  m_VAO.setAttributeArray(0, 3, sizeof(Vertex));
  m_VAO.setAttributeArray(1, 3, sizeof(Vertex), offsetof(Vertex, normal));
  m_VAO.setAttributeArray(2, 2, sizeof(Vertex), offsetof(Vertex, texCoords));

  m_VAO.unbind();
}