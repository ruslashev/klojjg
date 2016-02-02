#include "world.hh"

#include "constants.hh"

#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <vector>

void world::deallocate_data()
{
  if (!data)
    return;
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      delete [] data[z][y];
  for (unsigned int z = 0; z < sz; z++)
    delete [] data[z];
  delete [] data;
}

world::world()
{
  sx = sy = sz = 0;
  data = nullptr;
}

world::~world()
{
  deallocate_data();
}

void world::load_from_file(std::string path)
{
  std::ifstream ifs(path.c_str());
  if (!ifs)
    die("failed to open world \"%s\"");

  std::string line;
  while (getline(ifs, line)) {
    // sscanf(line.c_str(), "v %d %d %d")
    if (line.substr(0, 2) == "v ") {
      std::istringstream s(line.substr(2));
      int x, y, z;
      s >> x; s >> y; s >> z;
      data[z][y][x] = { 1 };
    }
  }
}

void world::set_size(unsigned int nsx, unsigned int nsy, unsigned int nsz)
{
  sx = nsx;
  sy = nsy;
  sz = nsz;
  // rip caches
  deallocate_data();
  data = new voxel** [sz];
  for (unsigned int z = 0; z < sz; z++)
    data[z] = new voxel* [sy];
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      data[z][y] = new voxel [sx];
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int x = 0; x < sx; x++)
        data[z][y][x] = { 0 }; // air
}

void world::generate_mesh()
{
  std::vector<GLfloat> positions;
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int x = 0; x < sx; x++)
        if (data[z][y][x].category == 1) {

        }
}

// vim: et:sw=2

