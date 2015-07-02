#include "Laser.h"


Laser::Laser()
{
  m_data[0] = glm::vec3(0.0f);
  m_data[1] = glm::vec3(0.0f);
}


void Laser::initialize()
{
  m_VBO.setUsage(GLTools::GLBufferObject::Usage::DynamicDraw);
  m_VAO.create();
  m_VBO.create();

  m_VAO.bind();

  m_VBO.bind();
  m_VBO.upload(m_data,
               2 * sizeof(glm::vec3));

  m_VAO.setAttributeArray(0, 3, sizeof(glm::vec3));
  m_VAO.unbind();
}

void Laser::draw(GLTools::GLShaderProgram& shaderProgram)
{
  m_VAO.bind();
  glDrawArrays(GL_LINES, 0, 2);
  m_VAO.unbind();
}

void Laser::commitDataChange()
{
  m_VAO.bind();
  m_VBO.write(0, m_data, 2 * sizeof(glm::vec3));
  m_VAO.unbind();
}

Laser::~Laser()
{
}
