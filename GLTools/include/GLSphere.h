#pragma once

#include "GLMesh.h"

namespace GLTools
{

class GLSphere : public GLMesh
{
public:
  GLSphere(float radius, int slices, int stacks);
  virtual ~GLSphere();
};

}
