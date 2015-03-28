#include "GLBufferObject.h"

GLBufferObject::GLBufferObject()
  : m_type(BufferType::VertexBuffer), m_usage(Usage::StaticDraw)
{
}

GLBufferObject::GLBufferObject(BufferType type)
  : m_type(type), m_usage(Usage::StaticDraw)
{
}

GLBufferObject::GLBufferObject(BufferType type, Usage usage)
  : m_type(type), m_usage(usage)
{
}

GLBufferObject::~GLBufferObject()
{
  destroy();
}

void GLBufferObject::bind()
{
  glBindBuffer(static_cast<GLenum>(m_type), m_id);
}

void GLBufferObject::unbind()
{
  glBindBuffer(static_cast<GLenum>(m_type), 0);
}

void GLBufferObject::create()
{
  glGenBuffers(1, &m_id);
}

void GLBufferObject::destroy()
{
  glDeleteBuffers(1, &m_id);
}

void GLBufferObject::upload(const void *data, size_t count)
{
  glBufferData(static_cast<GLenum>(m_type), count, data,
               static_cast<GLenum>(m_usage));
}

void GLBufferObject::write(size_t offset, const void *data, size_t count)
{
  glBufferSubData(static_cast<GLenum>(m_type), offset, count, data);
}
