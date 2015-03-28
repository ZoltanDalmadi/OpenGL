#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class GLVertexArrayObject
{
public:
  virtual ~GLVertexArrayObject();

  void create();
  void destroy();
  void bind();
  void unbind();
  GLuint ID() const;

private:
  GLuint m_id;
};

inline GLuint GLVertexArrayObject::ID() const
{
  return m_id;
}
