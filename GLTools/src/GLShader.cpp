#include "GLShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLTools::GLShader::GLShader(shaderType type)
  : m_type(type)
{
  GLuint gl_type = 0;

  switch (m_type)
  {
  case shaderType::VERTEX_SHADER:
    gl_type = GL_VERTEX_SHADER;
    break;

  case shaderType::FRAGMENT_SHADER:
    gl_type = GL_FRAGMENT_SHADER;
    break;

  case shaderType::GEOMETRY_SHADER:
    gl_type = GL_GEOMETRY_SHADER;
    break;
  }

  m_id = glCreateShader(gl_type);
}

GLTools::GLShader::GLShader(shaderType type, const std::string& path)
  : GLShader(type)
{
  if (loadSource(path))
    compile();
}

GLTools::GLShader::~GLShader()
{
  glDeleteShader(m_id);
}

void GLTools::GLShader::addSource(const std::string& source)
{
  m_sourceCode = source;

  const char *c_str = source.c_str();
  glShaderSource(m_id, 1, &c_str, nullptr);
}

bool GLTools::GLShader::loadSource(const std::string& filePath)
{
  std::stringstream buffer;

  std::ifstream sourceFile;
  sourceFile.exceptions(std::ifstream::failbit);

  try
  {
    sourceFile.open(filePath);
    buffer << sourceFile.rdbuf();
    addSource(buffer.str());
  }
  catch (std::ifstream::failure e)
  {
    std::cerr << "Could not find shader file: " << filePath << std::endl;
    return false;
  }

  sourceFile.close();

  return true;
}

bool GLTools::GLShader::compile()
{
  glCompileShader(m_id);

  GLint success;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
  m_compileSuccess = success ? true : false;

  char infolog[512];
  glGetShaderInfoLog(m_id, 512, nullptr, infolog);
  m_infoLog = infolog;

  return m_compileSuccess;
}
