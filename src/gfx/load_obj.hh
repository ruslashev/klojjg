#ifndef LOAD_OBJ_HH
#define LOAD_OBJ_HH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace load
{

void LoadOBJ(const char *filepath, std::vector<glm::vec3> *positions,
    std::vector<glm::vec3> *normals, std::vector<GLushort> *elements);

}

#endif

// vim: et:sw=2

