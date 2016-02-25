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
    std::vector<GLushort> &elements, std::vector<GLfloat> &texcoords)
{
  unsigned short n = 0;
  for (unsigned int z = 0; z < sz; z++)
    for (unsigned int y = 0; y < sy; y++)
      for (unsigned int x = 0; x < sx; x++)
        if (data[z][y][x].category == 1) {
          float xs = x*Constants.block_scale, xsp1 = (x+1)*Constants.block_scale,
                ys = y*Constants.block_scale, ysp1 = (y+1)*Constants.block_scale,
                zs = z*Constants.block_scale, zsp1 = (z+1)*Constants.block_scale;
          int num_faces = 0;
          if (z == 0 || (z > 0 && data[z-1][y][x].category == 0)) {
            push_vector_to_vector(positions, {
                xsp1, ys,   zs,   // back
                xs,   ys,   zs,
                xs,   ysp1, zs,
                xsp1, ysp1, zs });
            num_faces++;
          }
          if (z == sz - 1 || (z < sz - 1 && data[z+1][y][x].category == 0)) {
            push_vector_to_vector(positions, {
                xs,   ys,   zsp1, // front
                xsp1, ys,   zsp1,
                xsp1, ysp1, zsp1,
                xs,   ysp1, zsp1 });
            num_faces++;
          }
          if (y == 0 || (y > 0 && data[z][y-1][x].category == 0)) {
            push_vector_to_vector(positions, {
                xs,   ys,   zs,   // bottom
                xsp1, ys,   zs,
                xsp1, ys,   zsp1,
                xs,   ys,   zsp1 });
            num_faces++;
          }
          if (y == sy - 1 || (y < sy - 1 && data[z][y+1][x].category == 0)) {
            push_vector_to_vector(positions, {
                xs,   ysp1, zsp1, // top
                xsp1, ysp1, zsp1,
                xsp1, ysp1, zs,
                xs,   ysp1, zs });
            num_faces++;
          }
          if (x == 0 || (x > 0 && data[z][y][x-1].category == 0)) {
            push_vector_to_vector(positions, {
                xs,   ys,   zs,   // left
                xs,   ys,   zsp1,
                xs,   ysp1, zsp1,
                xs,   ysp1, zs });
            num_faces++;
          }
          if (x == sx - 1 || (x < sx - 1 && data[z][y][x+1].category == 0)) {
            push_vector_to_vector(positions, {
                xsp1, ys,   zsp1, // right
                xsp1, ys,   zs,
                xsp1, ysp1, zs,
                xsp1, ysp1, zsp1 });
            num_faces++;
          }
          for (int i = 0; i < num_faces; i++) {
            push_vector_to_vector(elements, {
                (GLushort)(n+0),  (GLushort)(n+1),  (GLushort)(n+2),
                (GLushort)(n+2),  (GLushort)(n+3),  (GLushort)(n+0) });
            n += 4;
            push_vector_to_vector(texcoords, { 0, 0, 1, 0, 1, 1, 0, 1 });
          }
        }
}

// vim: et:sw=2

