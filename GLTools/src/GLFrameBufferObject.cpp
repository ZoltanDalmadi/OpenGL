#include "GLFrameBufferObject.h"

GLTools::GLFrameBufferObject::GLFrameBufferObject()
{}

GLTools::GLFrameBufferObject::~GLFrameBufferObject()
{
  destroy();
}

void GLTools::GLFrameBufferObject::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void GLTools::GLFrameBufferObject::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLTools::GLFrameBufferObject::create()
{
  glGenFramebuffers(1, &m_id);
}

void GLTools::GLFrameBufferObject::destroy()
{
  glDeleteFramebuffers(1, &m_id);
}

void GLTools::GLFrameBufferObject::attachColorTexture(const GLTexture& texture)
{
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_id, 0);
}

void GLTools::GLFrameBufferObject::attachDepthTexture(const GLTexture& texture)
{
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.m_id, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
