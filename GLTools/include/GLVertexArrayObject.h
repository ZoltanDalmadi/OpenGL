#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace GLTools
{

class GLVertexArrayObject
{
public:
  virtual ~GLVertexArrayObject();

  void create();
  void destroy();
  void bind();
  void unbind();
  GLuint ID() const;

  void setAttributeArray
  (GLint location, GLint size, GLsizei stride, size_t offset = 0);

private:
  GLuint m_id;
};

inline GLuint GLVertexArrayObject::ID() const
{
  return m_id;
}

}