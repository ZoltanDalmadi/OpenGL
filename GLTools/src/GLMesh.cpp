#include "GLMesh.h"

GLTools::GLMesh::GLMesh()
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);
}

GLTools::GLMesh::GLMesh(std::vector<Vertex>& v, std::vector<GLuint>& i)
  : m_vertices(move(v)),
    m_indices(move(i))
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);
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

void GLTools::GLMesh::draw(const GLShaderProgram& shaderProgram)
{
  if (m_material->m_diffuseTex)
    m_material->m_diffuseTex->bind(m_material->m_diffuseTex_index);

  if (m_material->m_specularTex)
    m_material->m_specularTex->bind(m_material->m_specularTex_index);

  m_material->setShaderUniform(shaderProgram);

  draw();
}

void GLTools::GLMesh::draw()
{
  m_VAO.bind();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                 GL_UNSIGNED_INT, nullptr);

  m_VAO.unbind();
}

void GLTools::GLMesh::setMaterialTextureIndices(int diffuseIndex,
                                                int specularIndex)
{
  m_material->m_diffuseTex_index = diffuseIndex;
  m_material->m_specularTex_index = specularIndex;
}
