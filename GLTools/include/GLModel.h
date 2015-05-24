#pragma once
#include <assimp/mesh.h>
#include <assimp/material.h>
#include "GLMesh.h"

namespace GLTools
{

class GLModel
{
public:
  std::vector<GLMesh> m_meshes;
  std::vector<GLMaterial> m_materials;

  GLModel();
  explicit GLModel(const std::string& path);
  virtual ~GLModel();

  void loadModel(const std::string& path);
  void draw(const GLShaderProgram& shaderProgram);

private:
  GLMesh loadMesh(const aiMesh *mesh);
  GLMaterial loadMaterial(const aiMaterial *mat);
};

}
