#include <glm/gtc/matrix_transform.hpp>
#include "GLBoundingBox.h"

GLTools::GLBoundingBox::GLBoundingBox()
{
  m_EBO.setType(GLBufferObject::BufferType::IndexBuffer);

  m_vertices[0] = glm::vec3(-0.5f, -0.5f, -0.5f);
  m_vertices[1] = glm::vec3(0.5f, -0.5f, -0.5f);
  m_vertices[2] = glm::vec3(0.5f, 0.5f, -0.5f);
  m_vertices[3] = glm::vec3(-0.5f, 0.5f, -0.5f);
  m_vertices[4] = glm::vec3(-0.5f, -0.5f, 0.5f);
  m_vertices[5] = glm::vec3(0.5f, -0.5f, 0.5f);
  m_vertices[6] = glm::vec3(0.5f, 0.5f, 0.5f);
  m_vertices[7] = glm::vec3(-0.5f, 0.5f, 0.5f);

  m_indices =
  {
    {
      0, 1, 2, 3,
      4, 5, 6, 7,
      0, 4, 1, 5, 2, 6, 3, 7
    }
  };
}

GLTools::GLBoundingBox::~GLBoundingBox()
{}

void GLTools::GLBoundingBox::initialize()
{
  m_VAO.create();
  m_VBO.create();
  m_EBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_vertices.data(), m_vertices.size() * sizeof(glm::vec3));
  m_EBO.bind();
  m_EBO.upload(m_indices.data(), m_indices.size() * sizeof(GLuint));

  m_VAO.setAttributeArray(0, 3, sizeof(glm::vec3));

  m_VAO.unbind();
}

void GLTools::GLBoundingBox::draw
(const GLShaderProgram& shaderProgram, const glm::vec3& center,
 const glm::vec3& size, const glm::mat4& modelMatrix)
{
  this->update(center, size);
  m_modelMatrix = modelMatrix * m_modelMatrix;
  shaderProgram.setUniformValue("model", m_modelMatrix);
  this->draw();
}

void GLTools::GLBoundingBox::draw()
{
  m_VAO.bind();
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);

  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 reinterpret_cast<GLvoid *>(4 * sizeof(GLuint)));

  glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT,
                 reinterpret_cast<GLvoid *>(8 * sizeof(GLuint)));
  m_VAO.unbind();
}

void GLTools::GLBoundingBox::update
(const glm::vec3& center, const glm::vec3& size)
{
  m_modelMatrix = translate(glm::mat4(), center);
  m_modelMatrix = scale(m_modelMatrix, size);
}
