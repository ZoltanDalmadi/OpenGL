#pragma once
#include "GLCamera.h"

namespace GLTools
{

class GLFPSCamera : public GLCamera
{
public:

  enum class Direction
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD_FLOOR,
    BACKWARD_FLOOR
  };

  glm::vec3 m_front;
  glm::vec3 m_right;
  float m_pitch;
  float m_yaw;
  float m_speed = 0.1f;
  float m_sensivity = 0.1f;

  explicit GLFPSCamera(const glm::vec3& position);
  virtual ~GLFPSCamera();

  void move(Direction dir);
  void rotate(float xoffset, float yoffset, bool constrain = true);

private:
  virtual void update() override;
};

}
