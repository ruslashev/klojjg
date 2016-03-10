#pragma once

#include "gl.hh"

#include "../../deps/imgui/imgui.h"
#include <GL/glew.h>
#include <memory>

class Gui
{
  ImFont *font;

  void checkShaderCompileSuccess(int shader);
  void checkProgramLinkSuccess(int program);
public:
  GLuint fontTexture;
  program *shader_program;
  int attribLocationTex, attribLocationProjMtx;
  int attribLocationPosition, attribLocationUV, attribLocationColor;
  unsigned int vboHandle, vaoHandle, elementsHandle;

  Gui();
  ~Gui();

  void MainMenuBar();
  void TabBar();
  bool BeginSettingsWindow();
  void WaveWindow(bool *shouldCompile);
  void WaveWindowFileOps();
  void CreateFontTexture(ImFont *imFont);
  void Update(int dt);
  void Draw();
};

// vim: et:sw=2

