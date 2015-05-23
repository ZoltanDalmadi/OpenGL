#pragma once
#include <glm/detail/type_vec3.hpp>
#include "GLTexture.h"
#include "GLShaderProgram.h"

namespace GLTools
{

class GLMaterial
{
public:
  glm::vec3 m_diffuse;
  std::unique_ptr<GLTexture> m_diffuseTex;
  int m_diffuseTex_index;
  float m_diffMix;
  glm::vec3 m_specular;
  std::unique_ptr<GLTexture> m_specularTex;
  int m_specularTex_index;
  float m_specMix;
  float m_shininess;

  GLMaterial();
  ~GLMaterial();

  void setShaderUniform(const GLShaderProgram& shaderProgram);
};

}
