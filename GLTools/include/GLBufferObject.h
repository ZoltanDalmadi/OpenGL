#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

class GLBufferObject
{
public:
  enum class BufferType
  {
    VERTEX_BUFFER,
    INDEX_BUFFER
  };

  enum class Usage
  {
    STATIC_DRAW,
    DYNAMIC_DRAW
  };

  GLBufferObject();
  GLBufferObject(BufferType type);
  GLBufferObject(BufferType type, Usage usage);
  virtual ~GLBufferObject();

  GLuint ID() const;
  BufferType type() const;
  Usage usage() const;

  void setType(BufferType type);
  void setUsage(Usage usage);

  void bind();
  void unbind();
  void upload(const void *data, size_t count);
  void write(size_t offset, const void *data, size_t count);

private:
  GLuint m_id;
  BufferType m_type;
  Usage m_usage;
  GLenum m_gltype;
  GLenum m_glusage;

  void setGLType();
  void setGLUsage();
};

inline GLuint GLBufferObject::ID() const
{
  return m_id;
}

inline GLBufferObject::BufferType GLBufferObject::type() const
{
  return m_type;
}

inline GLBufferObject::Usage GLBufferObject::usage() const
{
  return m_usage;
}

inline void GLBufferObject::setType(GLBufferObject::BufferType type)
{
  this->m_type = type;
}

inline void GLBufferObject::setUsage(GLBufferObject::Usage usage)
{
  this->m_usage = usage;
}
