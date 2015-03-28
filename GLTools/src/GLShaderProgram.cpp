#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram()
{
}

GLShaderProgram::~GLShaderProgram()
{
  destroy();
}

bool GLShaderProgram::create()
{
  this->m_id = glCreateProgram();
  return m_id ? true : false;
}

void GLShaderProgram::destroy()
{
  glDeleteProgram(m_id);
}

void GLShaderProgram::addShader(const std::shared_ptr<GLShader>& shader)
{
  glAttachShader(m_id, shader->ID());
  this->m_shaders.push_back(shader);
  this->m_linkSuccess = false;
}

bool GLShaderProgram::link()
{
  glLinkProgram(m_id);

  GLint success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  m_linkSuccess = success ? true : false;

  if (!m_linkSuccess)
  {
    char infolog[512];
    glGetProgramInfoLog(m_id, 512, nullptr, infolog);
    m_infoLog = infolog;
  }

  return m_linkSuccess;
}

void GLShaderProgram::use() const
{
  glUseProgram(m_id);
}

void GLShaderProgram::setAttributeArray
(GLint location, GLint size, size_t offset, GLsizei stride)
{
  glVertexAttribPointer
  (location, size, GL_FLOAT, GL_FALSE, stride,
   reinterpret_cast<GLvoid *>(offset));
  glEnableVertexAttribArray(location);
}

GLint GLShaderProgram::uniformLocation(const char *name) const
{
  return glGetUniformLocation(m_id, name);
}

GLint GLShaderProgram::uniformLocation(const std::string& name) const
{
  return uniformLocation(name.c_str());
}

void GLShaderProgram::setUniformValue(GLint location, float x, float y)
{
  glUniform2f(location, x, y);
}

void GLShaderProgram::setUniformValue(const char *name, float x, float y)
{
  setUniformValue(uniformLocation(name), x, y);
}

void GLShaderProgram::setUniformValue(GLint location, const glm::vec2& value)
{
  setUniformValue(location, value.x, value.y);
}

void GLShaderProgram::setUniformValue(const char *name, const glm::vec2& value)
{
  setUniformValue(uniformLocation(name), value.x, value.y);
}

void GLShaderProgram::setUniformValue(GLint location, float x, float y, float z)
{
  glUniform3f(location, x, y, z);
}

void GLShaderProgram::setUniformValue(const char *name, float x, float y,
                                      float z)
{
  setUniformValue(uniformLocation(name), x, y, z);
}

void GLShaderProgram::setUniformValue(GLint location, const glm::vec3& value)
{
  setUniformValue(location, value.x, value.y, value.z);
}

void GLShaderProgram::setUniformValue(const char *name, const glm::vec3& value)
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z);
}

void GLShaderProgram::setUniformValue(GLint location, float x, float y, float z,
                                      float w)
{
  glUniform4f(location, x, y, z, w);
}

void GLShaderProgram::setUniformValue(const char *name, float x, float y,
                                      float z, float w)
{
  setUniformValue(uniformLocation(name), x, y, z, w);
}

void GLShaderProgram::setUniformValue(GLint location, const glm::vec4& value)
{
  setUniformValue(location, value.x, value.y, value.z, value.w);
}

void GLShaderProgram::setUniformValue(const char *name, const glm::vec4& value)
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z, value.w);
}

void GLShaderProgram::setUniformValue
(GLint location, const glm::mat3& value, GLboolean transpose)
{
  glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
}

void GLShaderProgram::setUniformValue
(const char *name, const glm::mat3& value, GLboolean transpose)
{
  setUniformValue(uniformLocation(name), value, transpose);
}

void GLShaderProgram::setUniformValue
(GLint location, const glm::mat4& value, GLboolean transpose)
{
  glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
}

void GLShaderProgram::setUniformValue
(const char *name, const glm::mat4& value, GLboolean transpose)
{
  setUniformValue(uniformLocation(name), value, transpose);
}
