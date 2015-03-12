#include "GLBufferObject.h"

GLBufferObject::GLBufferObject()
  : m_type(BufferType::VERTEX_BUFFER), m_usage(Usage::STATIC_DRAW)
{
  glGenBuffers(1, &m_id);
  setGLType();
  setGLUsage();
}

GLBufferObject::GLBufferObject(BufferType type)
  : m_type(type), m_usage(Usage::STATIC_DRAW)
{
  glGenBuffers(1, &m_id);
  setGLType();
  setGLUsage();
}

GLBufferObject::GLBufferObject(BufferType type, Usage usage)
  : m_type(type), m_usage(usage)
{
  glGenBuffers(1, &m_id);
  setGLType();
  setGLUsage();
}

GLBufferObject::~GLBufferObject()
{
  glDeleteBuffers(1, &m_id);
}

void GLBufferObject::bind()
{
  glBindBuffer(m_gltype, m_id);
}

void GLBufferObject::unbind()
{
  glBindBuffer(m_gltype, 0);
}

void GLBufferObject::upload(const void *data, size_t count)
{
  glBufferData(m_gltype, count, data, m_glusage);
}

void GLBufferObject::write(size_t offset, const void *data, size_t count)
{
  glBufferSubData(m_gltype, offset, count, data);
}

void GLBufferObject::setGLType()
{
  switch (m_type)
  {
  case BufferType::VERTEX_BUFFER:
    m_gltype = GL_ARRAY_BUFFER;
    break;

  case BufferType::INDEX_BUFFER:
    m_gltype = GL_ELEMENT_ARRAY_BUFFER;
    break;
  }
}

void GLBufferObject::setGLUsage()
{
  switch (m_usage)
  {
  case Usage::STATIC_DRAW:
    m_glusage = GL_STATIC_DRAW;
    break;

  case Usage::DYNAMIC_DRAW:
    m_glusage = GL_DYNAMIC_DRAW;
    break;
  }
}
