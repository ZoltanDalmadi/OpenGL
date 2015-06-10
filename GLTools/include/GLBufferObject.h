#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLTools
{

class GLBufferObject
{
public:
  enum class BufferType
  {
    VertexBuffer = 0x8892,
    IndexBuffer = 0x8893
  };

  enum class Usage
  {
    StaticDraw = 0x88E4,
    DynamicDraw = 0x88E8
  };

  explicit GLBufferObject(BufferType type = BufferType::VertexBuffer,
                          Usage usage = Usage::StaticDraw);
  virtual ~GLBufferObject();

  GLuint ID() const;
  BufferType type() const;
  Usage usage() const;

  void setType(BufferType type);
  void setUsage(Usage usage);

  void bind();
  void unbind();
  void create();
  void destroy();
  void upload(const void *data, size_t count);
  void write(size_t offset, const void *data, size_t count);

private:
  GLuint m_id;
  BufferType m_type;
  Usage m_usage;
};

}

inline GLuint GLTools::GLBufferObject::ID() const
{
  return m_id;
}

inline GLTools::GLBufferObject::BufferType GLTools::GLBufferObject::type() const
{
  return m_type;
}

inline GLTools::GLBufferObject::Usage GLTools::GLBufferObject::usage() const
{
  return m_usage;
}

inline void GLTools::GLBufferObject::setType
(GLTools::GLBufferObject::BufferType type)
{
  this->m_type = type;
}

inline void GLTools::GLBufferObject::setUsage
(GLTools::GLBufferObject::Usage usage)
{
  this->m_usage = usage;
}
