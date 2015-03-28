#include "GLVertexArrayObject.h"

GLVertexArrayObject::~GLVertexArrayObject()
{
  destroy();
}

void GLVertexArrayObject::create()
{
  glGenVertexArrays(1, &m_id);
}

void GLVertexArrayObject::destroy()
{
  glDeleteVertexArrays(1, &m_id);
}

void GLVertexArrayObject::bind()
{
  glBindVertexArray(m_id);
}

void GLVertexArrayObject::unbind()
{
  glBindVertexArray(0);
}
