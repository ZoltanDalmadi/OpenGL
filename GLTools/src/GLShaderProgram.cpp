#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram()
  : m_id(glCreateProgram())
{
}

GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::addShader(const GLShader& shader)
{
  glAttachShader(m_id, shader.ID());
}

bool GLShaderProgram::link()
{
  glLinkProgram(m_id);

  GLint success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  m_linkSuccess = success ? true : false;

  char infolog[512];
  glGetProgramInfoLog(m_id, 512, nullptr, infolog);
  m_infoLog = infolog;

  return m_linkSuccess;
}

void GLShaderProgram::use() const
{
  glUseProgram(m_id);
}
