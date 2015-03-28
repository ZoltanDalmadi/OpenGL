#pragma once
#include <glm/glm.hpp>

namespace GLTools
{

class GLFPSCamera
{
public:
  using vec3 = glm::vec3;
  using mat4 = glm::mat4;

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

  GLFPSCamera(const vec3& position);
  virtual ~GLFPSCamera();

  void move(Direction dir);
  void rotate(float xoffset, float yoffset, bool constrain = true);

private:
  void update();
};

}
