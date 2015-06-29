#pragma once
#include <memory>

#include "GLPlane.h"

class GridPlane
{
public:
  GridPlane();
  ~GridPlane();


private:
  std::unique_ptr<GLTools::GLPlane> plane;
};

