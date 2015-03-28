#include "GLVertexArrayObject.h"

GLTools::GLVertexArrayObject::~GLVertexArrayObject()
{
  destroy();
}

void GLTools::GLVertexArrayObject::create()
{
  glGenVertexArrays(1, &m_id);
}

void GLTools::GLVertexArrayObject::destroy()
{
  glDeleteVertexArrays(1, &m_id);
}

void GLTools::GLVertexArrayObject::bind()
{
  glBindVertexArray(m_id);
}

void GLTools::GLVertexArrayObject::unbind()
{
  glBindVertexArray(0);
}
