#pragma once

#include "GLMesh.h"

namespace GLTools
{

struct SimpleVertex
{
  glm::vec3 position;
};

class GLSkyBox : public GLMesh
{
public:
  std::vector<SimpleVertex> m_vertices;

  GLSkyBox(float size = 1.0f);
  virtual ~GLSkyBox();
  virtual void initialize();
};

}
