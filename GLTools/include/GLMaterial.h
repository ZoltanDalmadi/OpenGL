#pragma once
#include <glm/detail/type_vec3.hpp>
#include "GLTexture.h"
#include "GLShaderProgram.h"

namespace GLTools
{

class GLMaterial
{
public:
  glm::vec3 m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
  glm::vec3 m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
  std::shared_ptr<GLTexture> m_diffuseTex;
  std::shared_ptr<GLTexture> m_specularTex;
  int m_diffuseTex_index = 1;
  int m_specularTex_index = 2;
  float m_specMix = 1.0f;
  float m_diffMix = 1.0f;
  float m_shininess = 32.0f;

  GLMaterial();
  ~GLMaterial();

  void setShaderUniform(const GLShaderProgram& shaderProgram);
};

}
