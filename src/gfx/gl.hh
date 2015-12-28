#ifndef GL_HH
#define GL_HH

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
  void use();
};

#endif

// vim: et:sw=2

