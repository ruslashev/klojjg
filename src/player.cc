#include "player.hh"

glm::mat4 Player::computeViewMatrix()
{
  const float pitch = glm::radians(view_angles.x), yaw = glm::radians(view_angles.y);
  view_direction =
    glm::vec3(cos(pitch) * sin(yaw),
        sin(pitch),
        cos(pitch) * cos(yaw));
  glm::vec3 up = glm::vec3(0, 1, 0);
  view_right = glm::normalize(glm::cross(view_direction, up));
  glm::vec3 camera_up = glm::cross(view_right, view_direction);
  return glm::lookAt(position, position + view_direction, camera_up);
}

// vim: et:sw=2

