#include "player.hh"

glm::mat4 Player::computeViewMatrix()
{
  const float pitch = glm::radians(angles.x), yaw = glm::radians(angles.y);
  rev_direction =
    glm::vec3(cos(yaw) * sin(pitch),
        sin(yaw),
        cos(yaw) * cos(pitch));
  glm::vec3 up = glm::vec3(0, 1, 0);
  camera_right = glm::normalize(glm::cross(up, direction));
  glm::vec3 camera_up = glm::cross(direction, camera_right);
  return glm::lookAt(position, position + direction, camera_up);
}

// vim: et:sw=2

