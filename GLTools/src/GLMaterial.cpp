#include "GLMaterial.h"

GLTools::GLMaterial::GLMaterial()
{}

GLTools::GLMaterial::~GLMaterial()
{}

void GLTools::GLMaterial::setShaderUniform(const GLShaderProgram& shaderProgram)
{
  shaderProgram.setUniformValue("material.diffuse", m_diffuse);
  shaderProgram.setUniformValue("material.diffMix", m_diffMix);
  shaderProgram.setUniformValue("material.specular", m_specular);
  shaderProgram.setUniformValue("material.specMix", m_specMix);
  shaderProgram.setUniformValue("material.shininess", m_shininess);
  shaderProgram.setUniformValue("material.diffuseTex", m_diffuseTex_index);
  shaderProgram.setUniformValue("material.specularTex", m_specularTex_index);
}