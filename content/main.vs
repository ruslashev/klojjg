#version 120

attribute vec3 coord3d;
uniform mat4 mat_transform;

void main(void)
{
  gl_Position = mat_transform * vec4(coord3d, 1.0);
}

