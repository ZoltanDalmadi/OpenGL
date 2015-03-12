#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class GLVertexArrayObject
{
public:
  GLVertexArrayObject();
  virtual ~GLVertexArrayObject();

  void bind();
  void unbind();
  GLuint ID() const;

  static void setVertexAttrib(GLuint index, GLint size, GLenum type,
                              GLboolean normalize, GLint stride, GLint offset);

private:
  GLuint m_id;
  static size_t getGLTypeSize(GLenum type);
};

inline GLuint GLVertexArrayObject::ID() const
{
  return m_id;
}
