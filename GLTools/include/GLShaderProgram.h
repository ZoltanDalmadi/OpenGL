#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLShader.h"

namespace GLTools
{

class GLShaderProgram
{
  using GLShader_ptr = std::shared_ptr<GLShader>;

public:
  GLShaderProgram();
  virtual ~GLShaderProgram();

  void addShader(const GLShader_ptr& shader);
  bool link();
  bool create();
  void destroy();
  void use() const;

  GLint uniformLocation(const char *name) const;
  GLint uniformLocation(const std::string& name) const;

  void setUniformValue(GLint location, int i);
  void setUniformValue(const char *name, int i);
  void setUniformValue(GLint location, float f);
  void setUniformValue(const char *name, float f);
  void setUniformValue(GLint location, float x, float y);
  void setUniformValue(const char *name, float x, float y);
  void setUniformValue(GLint location, const glm::vec2& value);
  void setUniformValue(const char *name, const glm::vec2& value);
  void setUniformValue(GLint location, float x, float y, float z);
  void setUniformValue(const char *name, float x, float y, float z);
  void setUniformValue(GLint location, const glm::vec3& value);
  void setUniformValue(const char *name, const glm::vec3& value);
  void setUniformValue(GLint location, float x, float y, float z, float w);
  void setUniformValue(const char *name, float x, float y, float z, float w);
  void setUniformValue(GLint location, const glm::vec4& value);
  void setUniformValue(const char *name, const glm::vec4& value);

  void setUniformValue
  (GLint location, const glm::mat3& value, GLboolean transpose = GL_FALSE);
  void setUniformValue
  (const char *name, const glm::mat3& value, GLboolean transpose = GL_FALSE);
  void setUniformValue
  (GLint location, const glm::mat4& value, GLboolean transpose = GL_FALSE);
  void setUniformValue
  (const char *name, const glm::mat4& value, GLboolean transpose = GL_FALSE);

  bool isLinked() const;
  const std::string& log() const;
  GLuint ID() const;
  std::vector<GLShader_ptr> shaders() const;

private:
  GLuint m_id;
  bool m_linkSuccess = false;
  std::string m_infoLog;
  std::vector<GLShader_ptr> m_shaders;
};

}

inline bool GLTools::GLShaderProgram::isLinked() const
{
  return m_linkSuccess;
}

inline const std::string& GLTools::GLShaderProgram::log() const
{
  return m_infoLog;
}

inline GLuint GLTools::GLShaderProgram::ID() const
{
  return m_id;
}

inline std::vector<GLTools::GLShaderProgram::GLShader_ptr>
GLTools::GLShaderProgram::shaders() const
{
  return m_shaders;
}
