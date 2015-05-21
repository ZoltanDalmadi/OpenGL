#pragma once

#include "GLMesh.h"

namespace GLTools
{

class GLPlane : public GLMesh
{
public:
  GLPlane(float width, float height);
  virtual ~GLPlane();
};

}
