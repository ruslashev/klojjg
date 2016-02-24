#ifndef PLAYER_HH
#define PLAYER_HH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player
{
public:
  glm::vec3 position;
  glm::quat rotation;

  glm::mat4 computeViewMatrix();
};

#endif

// vim: et:sw=2

