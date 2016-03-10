#pragma once

#include <GL/glew.h>

struct buffer {
  GLuint id;
  buffer(GLenum target, GLsizei size, const void *data);
  ~buffer();
};

struct program {
  GLuint id;
  program(const char *vert_path, const char *frag_path);
  ~program();
  GLint bind_attribute(const char *name);
  GLint bind_uniform(const char *name);
  void use();
};

// vim: et:sw=2

