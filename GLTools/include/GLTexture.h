#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

namespace GLTools
{

class GLTexture
{
public:
  enum class TextureType
  {
    Texture2D = 0x0DE1,
    CubeMap = 0x8513
  };

  GLTexture(TextureType type = TextureType::Texture2D);
  GLTexture(const char *path);
  GLTexture(const char *faces[]);
  virtual ~GLTexture();

  void destroy();

  void bind() const;
  void bind(GLint unit) const;
  void unbind() const;

  void loadTexture2D(const char *path);
  void loadCubemap(const char *faces[]);

  GLuint m_id;
  TextureType m_type;
};

}