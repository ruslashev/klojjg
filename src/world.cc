#include "world.hh"

#include "constants.hh"

#include <fstream>
#include <sstream>

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
    if (line.substr(0, 2) == "v ") {
      std::istringstream s(line.substr(2));
      int x, y, z;
      s >> x; s >> y; s >> z;
      data[z][y][x] = { 1 };
    } else if (line.substr(0, 2) == "d ") {
      std::istringstream s(line.substr(2));
      int rsx, rsy, rsz;
      s >> rsx; s >> rsy; s >> rsz;
      set_size(rsx, rsy, rsz);
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

template <typename T>
void push_vector_to_vector(std::vector<T> &to, std::vector<T> what)
{
  for (auto &e : what)
    to.push_back(e);
}

void world::generate_mesh(std::vector<GLfloat> &positions,
    std::vector<GLushort> &elements)
{
  unsigned short n = 0;
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int x = 0; x < sx; x++)
        if (data[z][y][x].category == 1) {
          float xf = x, yf = y, zf = z; // avoiding compiler and lint warnings
          push_vector_to_vector(positions,
              { xf,   yf,   zf,
                xf+1, yf,   zf,
                xf,   yf+1, zf,
                xf+1, yf+1, zf,
                xf,   yf,   zf+1,
                xf+1, yf,   zf+1,
                xf,   yf+1, zf+1,
                xf+1, yf+1, zf+1 });
          push_vector_to_vector(elements, // linter pls
              { (GLushort)(n+0), (GLushort)(n+1), (GLushort)(n+2), // bottom
                (GLushort)(n+1), (GLushort)(n+2), (GLushort)(n+3),
                (GLushort)(n+4), (GLushort)(n+5), (GLushort)(n+6), // top
                (GLushort)(n+5), (GLushort)(n+6), (GLushort)(n+7),
                (GLushort)(n+7), (GLushort)(n+5), (GLushort)(n+1), // right
                (GLushort)(n+1), (GLushort)(n+7), (GLushort)(n+3),
                (GLushort)(n+4), (GLushort)(n+2), (GLushort)(n+0), // blaze it
                (GLushort)(n+2), (GLushort)(n+6), (GLushort)(n+4),
                (GLushort)(n+2), (GLushort)(n+3), (GLushort)(n+7), // front
                (GLushort)(n+2), (GLushort)(n+6), (GLushort)(n+7),
                (GLushort)(n+4), (GLushort)(n+5), (GLushort)(n+0), // back
                (GLushort)(n+0), (GLushort)(n+1), (GLushort)(n+5) });
          n += 8;
        }
}

// vim: et:sw=2

