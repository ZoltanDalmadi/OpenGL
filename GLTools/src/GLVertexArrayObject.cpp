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

void GLVertexArrayObject::setVertexAttrib(
  GLuint index, GLint size, GLenum type, GLboolean normalize, GLint stride,
  GLint offset)
{
  size_t typeSize = getGLTypeSize(type);
  glVertexAttribPointer(index, size, type, normalize,
                        static_cast<GLsizei>(stride * typeSize),
                        reinterpret_cast<GLvoid *>(offset * typeSize));
  glEnableVertexAttribArray(index);
}

size_t GLVertexArrayObject::getGLTypeSize(GLenum type)
{
  size_t size;

  switch (type)
  {
  case GL_FLOAT:
    size = sizeof(GLfloat);
    break;

  case GL_DOUBLE:
    size = sizeof(GLdouble);
    break;
  }

  return size;
}
