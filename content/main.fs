#version 120

varying vec2 f_texcoord;
uniform sampler2D texid;

void main(void)
{
  gl_FragColor = texture2D(texid, f_texcoord);
}

