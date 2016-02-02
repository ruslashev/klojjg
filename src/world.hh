#ifndef WORLD_HH
#define WORLD_HH

#include <string>

struct voxel
{
  // zero or less -> non-walkable
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
  void generate_mesh();
};

#endif

// vim: et:sw=2

