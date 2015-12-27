#include "load_obj.hh"

#include "../constants.hh"
#include <fstream>

void load::LoadOBJ(const char *filepath, std::vector<glm::vec3> positions,
    std::vector<glm::vec3> normals, std::vector<GLushort> elements)
{
  std::ifstream in(filepath, std::ios::in);
  if (!in)
    die("failed to open file \"%s\"", filepath);
  size_t len = in.tellg();
  in.seekg(0, std::ios::beg);
  std::vector<char> file(len);
  in.read(&file[0], len);
  in.close();

  std::vector<glm::vec3> loc_positions, loc_normals;
  std::vector<GLushort> loc_elements;

  int current = 0, lineno = 1;
  auto ch = [&]() { return file[current]; };
  auto eat = [&]() { current++; };
  if (ch() == '#') {
    while (ch() != '\n')
      eat;
    eat;
    lineno++;
  } else if (ch() == 'v') {
    eat;
    if (ch() == 'p')
      die("vp: not implemented");
    else {
      bool normal = (ch() == 'n');
      double nums[3];
      for (int n = 0; n < 3; n++) {
        char numbuf[20];
        int i = 0;
        while (ch() == ' ')
          eat;
        while ((ch() > '0' && ch() < '9') || ch() == '.' || ch() == '-') {
          numbuf[i++] = ch();
          eat;
        }
        numbuf[i] = 0;
        nums[n] = atof(numbuf);
      }
      if (normal)
        loc_normals.push_back(glm::vec3(nums[0], nums[1], nums[2]));
      else
        loc_positions.push_back(glm::vec3(nums[0], nums[1], nums[2]));
    }
    while (ch() == ' ')
      eat;
    if (ch() != '\n')
      die("\"%s\" at line %d: error parsing vertex", filepath, lineno);
    eat;
  } else if (ch() == 'f') {
    eat;
    for (int n = 0; n < 3; n++) {
      char numbuf[20];
      int i = 0;
      while (ch() == ' ')
        eat;
      while (ch() > '0' && ch() < '9') {
        numbuf[i++] = ch();
        eat;
      }
      numbuf[i] = 0;
      nums[n] = atoi(numbuf);
    }
  }
}

// vim: et:sw=2

