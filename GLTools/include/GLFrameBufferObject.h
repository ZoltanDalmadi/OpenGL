#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include "GLTexture.h"

namespace GLTools
{

class GLFrameBufferObject
{
public:
  GLFrameBufferObject();
  virtual ~GLFrameBufferObject();

  GLuint ID() const
  {
    return m_id;
  }

  void bind();
  void unbind();
  void create();
  void destroy();
  void attachColorTexture(const GLTexture& texture);
  void attachDepthTexture(const GLTexture& texture);

private:
  GLuint m_id;
};

}
