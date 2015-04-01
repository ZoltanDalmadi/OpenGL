#include "GLShaderProgram.h"

GLTools::GLShaderProgram::GLShaderProgram()
{
}

GLTools::GLShaderProgram::~GLShaderProgram()
{
  destroy();
}

bool GLTools::GLShaderProgram::create()
{
  this->m_id = glCreateProgram();
  return m_id ? true : false;
}

void GLTools::GLShaderProgram::destroy()
{
  glDeleteProgram(m_id);
}

void GLTools::GLShaderProgram::addShader(const std::shared_ptr<GLShader>&
                                         shader)
{
  glAttachShader(m_id, shader->ID());
  this->m_shaders.push_back(shader);
  this->m_linkSuccess = false;
}

bool GLTools::GLShaderProgram::link()
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

void GLTools::GLShaderProgram::use() const
{
  glUseProgram(m_id);
}

GLint GLTools::GLShaderProgram::uniformLocation(const char *name) const
{
  return glGetUniformLocation(m_id, name);
}

GLint GLTools::GLShaderProgram::uniformLocation(const std::string& name) const
{
  return uniformLocation(name.c_str());
}

void GLTools::GLShaderProgram::setUniformValue(GLint location, int i) const
{
  glUniform1i(location, i);
}

void GLTools::GLShaderProgram::setUniformValue(const char *name, int i) const
{
  setUniformValue(uniformLocation(name), i);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, int i) const
{
  setUniformValue(uniformLocation(name), i);
}

void GLTools::GLShaderProgram::setUniformValue(GLint location, float f) const
{
  glUniform1f(location, f);
}

void GLTools::GLShaderProgram::setUniformValue(const char *name, float f) const
{
  setUniformValue(uniformLocation(name), f);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, float f) const
{
  setUniformValue(uniformLocation(name), f);
}

void GLTools::GLShaderProgram::setUniformValue(GLint location, float x,
                                               float y) const
{
  glUniform2f(location, x, y);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, float x, float y) const
{
  setUniformValue(uniformLocation(name), x, y);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, float x, float y) const
{
  setUniformValue(uniformLocation(name), x, y);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, const glm::vec2& value) const
{
  setUniformValue(location, value.x, value.y);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, const glm::vec2& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, const glm::vec2& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, float x, float y, float z) const
{
  glUniform3f(location, x, y, z);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, float x, float y, float z) const
{
  setUniformValue(uniformLocation(name), x, y, z);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, float x, float y, float z) const
{
  setUniformValue(uniformLocation(name), x, y, z);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, const glm::vec3& value) const
{
  setUniformValue(location, value.x, value.y, value.z);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, const glm::vec3& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, const glm::vec3& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, float x, float y, float z, float w) const
{
  glUniform4f(location, x, y, z, w);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, float x, float y, float z, float w) const
{
  setUniformValue(uniformLocation(name), x, y, z, w);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, float x, float y, float z, float w) const
{
  setUniformValue(uniformLocation(name), x, y, z, w);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, const glm::vec4& value) const
{
  setUniformValue(location, value.x, value.y, value.z, value.w);
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, const glm::vec4& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z, value.w);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, const glm::vec4& value) const
{
  setUniformValue(uniformLocation(name), value.x, value.y, value.z, value.w);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, const glm::mat3& value, GLboolean transpose) const
{
  glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, const glm::mat3& value, GLboolean transpose) const
{
  setUniformValue(uniformLocation(name), value, transpose);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, const glm::mat3& value, GLboolean transpose) const
{
  setUniformValue(uniformLocation(name), value, transpose);
}

void GLTools::GLShaderProgram::setUniformValue
(GLint location, const glm::mat4& value, GLboolean transpose) const
{
  glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
}

void GLTools::GLShaderProgram::setUniformValue
(const char *name, const glm::mat4& value, GLboolean transpose) const
{
  setUniformValue(uniformLocation(name), value, transpose);
}

void GLTools::GLShaderProgram::setUniformValue
(const std::string& name, const glm::mat4& value, GLboolean transpose) const
{
  setUniformValue(uniformLocation(name), value, transpose);
}
