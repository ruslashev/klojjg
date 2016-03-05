#include "gui.hh"
#include "../constants.hh"
#include "../state_game.hh"
#include <GLFW/glfw3.h>

static void ImGuiRenderDrawLists(ImDrawData *draw_data)
{
  const Gui *gui = (Gui*)ImGui::GetIO().UserData;
  // Backup GL state
  GLint last_program, last_texture, last_array_buffer,
        last_element_array_buffer, last_vertex_array, last_blend_src,
        last_blend_dst, last_blend_equation_rgb, last_blend_equation_alpha,
        last_viewport[4];
  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program),
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
  glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
  glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
  glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLboolean last_enable_blend = glIsEnabled(GL_BLEND),
            last_enable_cull_face = glIsEnabled(GL_CULL_FACE),
            last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST),
            last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

  // Setup render state: alpha-blending enabled, no face culling,
  // no depth testing, scissor enabled
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);

  // Handle cases of screen coordinates != from framebuffer coordinates
  // (e.g. retina displays)
  ImGuiIO& io = ImGui::GetIO();
  int fb_width = (int)(io.DisplaySize.x*io.DisplayFramebufferScale.x);
  int fb_height = (int)(io.DisplaySize.y*io.DisplayFramebufferScale.y);
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // Setup viewport, orthographic projection matrix
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  const float ortho_projection[4][4] = {
    { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
    { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
    { 0.0f,                  0.0f,                  -1.0f, 0.0f },
    {-1.0f,                  1.0f,                   0.0f, 1.0f },
  };
  glUseProgram(gui->shaderHandle);
  glUniform1i(gui->attribLocationTex, 0);
  glUniformMatrix4fv(gui->attribLocationProjMtx, 1, GL_FALSE,
      &ortho_projection[0][0]);
  glBindVertexArray(gui->vaoHandle);

  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList *cmd_list = draw_data->CmdLists[n];
    const ImDrawIdx *idx_buffer_offset = nullptr;

    glBindBuffer(GL_ARRAY_BUFFER, gui->vboHandle);
    glBufferData(GL_ARRAY_BUFFER,
        (GLsizeiptr)cmd_list->VtxBuffer.size()*sizeof(ImDrawVert),
        (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui->elementsHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        (GLsizeiptr)cmd_list->IdxBuffer.size()*sizeof(ImDrawIdx),
        (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

    for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin();
        pcmd != cmd_list->CmdBuffer.end(); pcmd++) {
      if (pcmd->UserCallback)
        pcmd->UserCallback(cmd_list, pcmd);
      else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
        glScissor((int)pcmd->ClipRect.x,
            (int)(fb_height - pcmd->ClipRect.w),
            (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
            (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            idx_buffer_offset);
      }
      idx_buffer_offset += pcmd->ElemCount;
    }
  }

  // Restore modified GL state
  glUseProgram(last_program);
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
  glBindVertexArray(last_vertex_array);
  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFunc(last_blend_src, last_blend_dst);
  if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
  if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
  if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
  if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
  glViewport(last_viewport[0], last_viewport[1],
      (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}

static const char* GetClipboardText() {
  return glfwGetClipboardString(Globals.glfwWindowPtr);
}
static void SetClipboardText(const char* text) {
  glfwSetClipboardString(Globals.glfwWindowPtr, text);
}

Gui::Gui()
{
  int err = glewInit();
  if (err != GLEW_OK)
    die("Failed to initialize GLEW: %s", glewGetErrorString(err));

  fontTexture = 0;
  shaderHandle = 0, vertHandle = 0, fragHandle = 0;
  attribLocationTex = 0, attribLocationProjMtx = 0;
  attribLocationPosition = 0, attribLocationUV = 0, attribLocationColor = 0;
  vboHandle = 0, vaoHandle = 0, elementsHandle = 0;

  ImGuiIO& io = ImGui::GetIO();
  io.RenderDrawListsFn = ImGuiRenderDrawLists;
  io.SetClipboardTextFn = SetClipboardText;
  io.GetClipboardTextFn = GetClipboardText;
  io.DisplaySize = ImVec2(Globals.windowWidth, Globals.windowHeight);
  io.IniFilename = NULL;
  io.UserData = this;
  io.KeyRepeatDelay = 5.5;
  io.KeyRepeatRate = 1.1;

  const GLchar *vertex_shader =
    "#version 120\n"
    "uniform mat4 ProjMtx;\n"
    "attribute vec2 Position;\n"
    "attribute vec2 UV;\n"
    "attribute vec4 Color;\n"
    "varying vec2 Frag_UV;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "	Frag_UV = UV;\n"
    "	Frag_Color = Color;\n"
    "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const GLchar* fragment_shader =
    "#version 120\n"
    "uniform sampler2D Texture;\n"
    "varying vec2 Frag_UV;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "	gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
    "}\n";

  shaderHandle = glCreateProgram();
  vertHandle = glCreateShader(GL_VERTEX_SHADER);
  fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertHandle, 1, &vertex_shader, 0);
  glShaderSource(fragHandle, 1, &fragment_shader, 0);
  glCompileShader(vertHandle);
  glCompileShader(fragHandle);
  glAttachShader(shaderHandle, vertHandle);
  glAttachShader(shaderHandle, fragHandle);
  glLinkProgram(shaderHandle);

  checkShaderCompileSuccess(vertHandle);
  checkShaderCompileSuccess(fragHandle);
  checkProgramLinkSuccess(shaderHandle);

  attribLocationTex = glGetUniformLocation(shaderHandle, "Texture");
  attribLocationProjMtx = glGetUniformLocation(shaderHandle, "ProjMtx");
  attribLocationPosition = glGetAttribLocation(shaderHandle, "Position");
  attribLocationUV = glGetAttribLocation(shaderHandle, "UV");
  attribLocationColor = glGetAttribLocation(shaderHandle, "Color");

  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &elementsHandle);

  glGenVertexArrays(1, &vaoHandle);
}

void Gui::checkShaderCompileSuccess(int shader)
{
  GLint status, type;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  glGetShaderiv(shader, GL_SHADER_TYPE, &type);
  if (status == GL_FALSE) {
    char buffer[1024];
    glGetShaderInfoLog(shader, 1024, NULL, buffer);
    die("Failed to compile %s shader:\n%s",
        type == GL_VERTEX_SHADER ? "vertex" : "fragment",
        buffer);
  }
}

void Gui::checkProgramLinkSuccess(int program)
{
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    char buffer[1024];
    glGetProgramInfoLog(vertHandle, 1024, NULL, buffer);
    die("Failed to link shaders:\n%s", buffer);
  }
}

void Gui::CreateFontTexture(ImFont *imFont)
{
  font = imFont;

  ImGuiIO& io = ImGui::GetIO();

  unsigned char *pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  glGenTextures(1, &fontTexture);
  glBindTexture(GL_TEXTURE_2D, fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, pixels);

  io.Fonts->TexID = (void*)(intptr_t)fontTexture;
}

void Gui::Update(int dt)
{
}

void Gui::Draw()
{
  glBindVertexArray(vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
  glEnableVertexAttribArray(attribLocationPosition);
  glEnableVertexAttribArray(attribLocationUV);
  glEnableVertexAttribArray(attribLocationColor);
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
  glVertexAttribPointer(attribLocationPosition, 2, GL_FLOAT, GL_FALSE,
      sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(attribLocationUV, 2, GL_FLOAT, GL_FALSE,
      sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE,
      sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  bool yes = true;
  ImGui::ShowTestWindow(&yes);

  ImGui::Begin("yes");
  if (ImGui::Button("go game men))"))
    Globals.stateDispatcherPtr->ChangeTo(&state_game);
  ImGui::End();

  ImGui::Render();
}

Gui::~Gui()
{
  if (vaoHandle) glDeleteVertexArrays(1, &vaoHandle);
  if (vboHandle) glDeleteBuffers(1, &vboHandle);
  if (elementsHandle) glDeleteBuffers(1, &elementsHandle);
  vaoHandle = vboHandle = elementsHandle = 0;

  glDetachShader(shaderHandle, vertHandle);
  glDeleteShader(vertHandle);
  vertHandle = 0;

  glDetachShader(shaderHandle, fragHandle);
  glDeleteShader(fragHandle);
  fragHandle = 0;

  glDeleteProgram(shaderHandle);
  shaderHandle = 0;

  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->ClearInputData();
  io.Fonts->ClearTexData();

  if (fontTexture) {
    glDeleteTextures(1, &fontTexture);
    ImGui::GetIO().Fonts->TexID = 0;
    fontTexture = 0;
  }
  ImGui::Shutdown();
}

// vim: et:sw=2

