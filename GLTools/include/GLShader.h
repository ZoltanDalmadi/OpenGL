#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

class GLShader
{
public:
  enum class shaderType
  {
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
  };

  GLShader(shaderType type);
  virtual ~GLShader();

  void addSource(const std::string& source);
  bool loadSource(const std::string& filePath);
  bool compile();

  bool isCompiled() const;
  const std::string& log() const;
  shaderType type() const;
  const std::string& source() const;
  GLuint ID() const;

private:
  bool m_compileSuccess;
  shaderType m_type;
  bool m_loadSuccess;
  GLint m_id;
  std::string m_infoLog;
  std::string m_sourceCode;
};

inline bool GLShader::isCompiled() const
{
  return m_compileSuccess;
}

inline const std::string& GLShader::log() const
{
  return m_infoLog;
}

inline GLShader::shaderType GLShader::type() const
{
  return m_type;
}

inline const std::string& GLShader::source() const
{
  return m_sourceCode;
}

inline GLuint GLShader::ID() const
{
  return m_id;
}
