#ifndef GUI_HH
#define GUI_HH

#include "../../deps/imgui/imgui.h"
#include <GL/glew.h>
#include <memory>

class Gui
{
  ImFont *font;
  char editingBuffer[16*1024];

  void checkShaderCompileSuccess(int shader);
  void checkProgramLinkSuccess(int program);
public:
  GLuint fontTexture;
  int shaderHandle, vertHandle, fragHandle;
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

#endif

// vim: et:sw=2

