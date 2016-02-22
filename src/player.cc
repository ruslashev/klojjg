#include "player.hh"

glm::mat4 Player::computeViewMatrix()
{
  direction = glm::vec3(cos(angles.x) * sin(angles.y),
      sin(angles.x),
      cos(angles.x) * cos(angles.y));
  glm::vec3 up = glm::vec3(0, 1, 0);
  cameraRight = glm::normalize(glm::cross(up, direction));
  glm::vec3 cameraUp = glm::cross(direction, cameraRight);
  return glm::lookAt(position, position + direction, cameraUp);
}

// vim: et:sw=2

