#pragma once
#include "GLShader.h"

class GLShaderProgram
{
public:
  GLShaderProgram();
  virtual ~GLShaderProgram();

  void addShader(const GLShader& shader);
  bool link();
  void use() const;

  bool isLinked() const;
  const std::string& log() const;
  GLuint ID() const;

private:
  GLuint m_id;
  bool m_linkSuccess = false;
  std::string m_infoLog;
};

inline bool GLShaderProgram::isLinked() const
{
  return m_linkSuccess;
}

inline const std::string& GLShaderProgram::log() const
{
  return m_infoLog;
}

inline GLuint GLShaderProgram::ID() const
{
  return m_id;
}
