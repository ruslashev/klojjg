#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player
{
public:
  glm::vec3 position;
  glm::vec2 view_angles; // rotations around x and y axes respectively (pitch and yaw)
  glm::vec3 view_direction, view_right;

  glm::mat4 computeViewMatrix();
};

// vim: et:sw=2

