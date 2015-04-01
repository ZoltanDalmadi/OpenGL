#pragma once

#include "GLMesh.h"

namespace GLTools
{

class GLCube : public GLMesh
{
public:
  GLCube(float size);
  virtual ~GLCube();
};

}
