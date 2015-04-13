#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

namespace GLTools
{

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
  GLShader(shaderType type, const std::string& path);
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

}

inline bool GLTools::GLShader::isCompiled() const
{
  return m_compileSuccess;
}

inline const std::string& GLTools::GLShader::log() const
{
  return m_infoLog;
}

inline GLTools::GLShader::shaderType GLTools::GLShader::type() const
{
  return m_type;
}

inline const std::string& GLTools::GLShader::source() const
{
  return m_sourceCode;
}

inline GLuint GLTools::GLShader::ID() const
{
  return m_id;
}
