#ifndef WORLD_HH
#define WORLD_HH

#include <GL/glew.h>
#include <string>
#include <vector>

struct voxel
{
  // 1 or more -> non-walkable
  int16_t category;
};

class world
{
  void deallocate_data();
public:
  unsigned int sx, sy, sz;
  voxel ***data;

  world();
  ~world();

  void load_from_file(std::string path);
  void set_size(unsigned int nsx, unsigned int nsy, unsigned int nsz);
  void generate_mesh(std::vector<GLfloat> &positions,
      std::vector<GLushort> &elements, std::vector<GLfloat> &texcoords);
};

#endif

// vim: et:sw=2

