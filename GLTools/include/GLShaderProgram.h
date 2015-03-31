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

  void setUniformValue(GLint, int) const;
  void setUniformValue(const char *, int) const;
  void setUniformValue(GLint, float) const;
  void setUniformValue(const char *, float) const;
  void setUniformValue(GLint, float, float) const;
  void setUniformValue(const char *, float, float) const;
  void setUniformValue(GLint, const glm::vec2&) const;
  void setUniformValue(const char *, const glm::vec2&) const;
  void setUniformValue(GLint, float, float, float) const;
  void setUniformValue(const char *, float, float, float) const;
  void setUniformValue(GLint, const glm::vec3&) const;
  void setUniformValue(const char *, const glm::vec3&) const;
  void setUniformValue(GLint, float, float, float, float) const;
  void setUniformValue(const char *, float, float, float, float) const;
  void setUniformValue(GLint, const glm::vec4&) const;
  void setUniformValue(const char *, const glm::vec4&) const;

  void setUniformValue
  (GLint, const glm::mat3&, GLboolean = GL_FALSE) const;
  void setUniformValue
  (const char *, const glm::mat3&, GLboolean = GL_FALSE) const;
  void setUniformValue
  (GLint, const glm::mat4&, GLboolean = GL_FALSE) const;
  void setUniformValue
  (const char *, const glm::mat4&, GLboolean = GL_FALSE) const;

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
