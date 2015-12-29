#include "load_obj.hh"

#include "../constants.hh"
#include <fstream>
#include <map>

void load::LoadOBJ(const char *filepath, std::vector<glm::vec3> *positions,
    std::vector<glm::vec3> *normals, std::vector<GLushort> *elements)
{
  FILE *f = fopen(filepath, "rb");
  if (!f)
    die("failed to open file \"%s\" for reading", filepath);
  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);
  char *file = new char [len];
  if (fread(file, 1, len, f) != len) {
    fclose(f);
    delete [] file;
    die("failed to read file \"%s\"", filepath);
  }
  fclose(f);

  std::vector<glm::vec3> loc_positions, loc_normals;
  std::vector<glm::vec2> loc_texcoords;
  std::vector<GLushort> loc_elements;
  std::map<GLushort,glm::vec3> shared_normals;

  int current = 0, lineno = 1;
  auto ch = [&]() { return file[current]; };
  auto eat = [&]() { current++; };
  auto eatspaces = [&]() { while (ch() == ' ' || ch() == '\t') eat(); };
  if (ch() == '#') {
    while (ch() != '\n')
      eat();
    eat();
    lineno++;
  } else if (ch() == 'v') {
    eat();
    if (ch() == 'p')
      die("vp: not implemented");
    else {
      bool normal = (ch() == 'n');
      double nums[3];
      for (int n = 0; n < 3; n++) {
        char numbuf[20];
        int i = 0;
        eatspaces();
        while ((ch() > '0' && ch() < '9') || ch() == '.' || ch() == '-') {
          numbuf[i++] = ch();
          eat();
        }
        numbuf[i] = 0;
        nums[n] = atof(numbuf);
      }
      if (normal)
        loc_normals.push_back(glm::vec3(nums[0], nums[1], nums[2]));
      else
        loc_positions.push_back(glm::vec3(nums[0], nums[1], nums[2]));
    }
    eatspaces();
    if (ch() != '\n')
      die("\"%s\" at line %d: error parsing vertex", filepath, lineno);
    eat();
  } else if (ch() == 'f') {
    eat();
    int vertex_idx[3], texcoord_idx[3], normal_idx[3];
    bool texcoords = false, normals = false;
    for (int n = 0; n < 3; n++) {
      char numbuf[20];
      eatspaces();
      int i = 0;
      while (ch() > '0' && ch() < '9') {
        numbuf[i++] = ch();
        eat();
      }
      numbuf[i] = 0;
      vertex_idx[n] = atoi(numbuf);

      if (ch() != '/')
        continue;
      eat();

      i = 0;
      while (ch() > '0' && ch() < '9') {
        numbuf[i++] = ch();
        eat();
      }
      numbuf[i] = 0;
      if (i != 0) {
        texcoord_idx[n] = atoi(numbuf);
        texcoords = true;
      }

      if (ch() != '/')
        die("\"%s\" at line %d: expected \"/\"");
      eat();

      i = 0;
      while (ch() > '0' && ch() < '9') {
        numbuf[i++] = ch();
        eat();
      }
      numbuf[i] = 0;
      if (i != 0) {
        normal_idx[n] = atoi(numbuf);
        normals = true;
      }
    }
    for (int n = 0; n < 3; n++) {
      const int element = vertex_idx[n];
      loc_elements.push_back(element);
      if (shared_normals.count(element))
        shared_normals[element] = glm::vec3(
            (shared_normals[element].x + normal_idx[0])/2.0,
            (shared_normals[element].y + normal_idx[1])/2.0,
            (shared_normals[element].z + normal_idx[2])/2.0);
      else
        shared_normals[element] = glm::vec3(
            normal_idx[0],
            normal_idx[1],
            normal_idx[2]);
    }
  }

  puts("wutv");
  for (auto v : loc_positions)
    printf("(%f %f %f) ", v.x, v.y, v.z);

  if (positions)
    *positions = (loc_positions);
  if (normals)
    *normals = (loc_normals);
  if (elements)
    *elements =(loc_elements);
}

// vim: et:sw=2

