#include "GLTexture.h"
#include <SOIL/SOIL.h>

GLTools::GLTexture::GLTexture(GLTexture::TextureType type)
  : m_type(type)
{}

GLTools::GLTexture::GLTexture(const char *path)
  : m_type(TextureType::Texture2D)
{
  loadTexture2D(path);
}

GLTools::GLTexture::GLTexture(const char *faces[])
  : m_type(TextureType::CubeMap)
{
  loadCubemap(faces);
}

GLTools::GLTexture::~GLTexture()
{
  destroy();
}

void GLTools::GLTexture::createColorTexture(GLsizei width, GLsizei height)
{
  if (m_type != TextureType::Texture2D)
    return;

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTools::GLTexture::createDepthTexture(GLsizei width, GLsizei height)
{
  if (m_type != TextureType::Texture2D)
    return;

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTools::GLTexture::destroy()
{
  glDeleteTextures(1, &m_id);
}

void GLTools::GLTexture::bind() const
{
  glBindTexture(static_cast<GLenum>(m_type), m_id);
}

void GLTools::GLTexture::bind(GLint unit) const
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(static_cast<GLenum>(m_type), m_id);
}

void GLTools::GLTexture::unbind() const
{
  glBindTexture(static_cast<GLenum>(m_type), 0);
}

void GLTools::GLTexture::loadTexture2D(const char *path)
{
  if (m_type != TextureType::Texture2D)
    return;

  glGenTextures(1, &m_id);
  int width, height;
  unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);
}

void GLTools::GLTexture::loadCubemap(const char *faces[])
{
  if (m_type != TextureType::CubeMap)
    return;

  glGenTextures(1, &m_id);
  glActiveTexture(GL_TEXTURE0);

  int width, height;
  unsigned char *image;

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

  for (GLuint i = 0; i < 6; i++)
  {
    image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
      GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
    );
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  SOIL_free_image_data(image);
}
