#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;
using cvec3r = const vec3&;
using cvec3r = const vec3&;

class GLFPSCamera
{
public:
  enum class Direction
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  vec3 m_position;
  vec3 m_front;
  vec3 m_up;
  vec3 m_right;
  float m_pitch;
  float m_yaw;
  float m_speed = 0.1f;
  float m_sensivity = 0.1f;

  mat4 m_viewMatrix;

  GLFPSCamera(cvec3r position);
  virtual ~GLFPSCamera();

  void move(Direction dir);
  void rotate(float xoffset, float yoffset, bool constrain = true);

private:
  void update();
};
