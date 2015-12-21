#include "constants.hh"
#include "font.hh"
#include "fontloader.hh"
#include "gui.hh"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

static void keyInputCb(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    Globals.quit = true;
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[key] = (action != GLFW_RELEASE);
}
static void mouseMoveCb(GLFWwindow *window, double xpos, double ypos)
{
  if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
    Globals.guiptr->mousePosX = xpos;
    Globals.guiptr->mousePosY = ypos;
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(xpos, ypos);
  }
}
static void mouseInputCb(GLFWwindow*, int button, int action, int)
{
  if (action == GLFW_PRESS && button >= 0 && button <= 3)
    Globals.guiptr->mousePressed[button] = true;
}
static void mouseScrollCb(GLFWwindow *window, double xoffset, double yoffset)
{
  ImGui::GetIO().MouseWheel += (float)yoffset;
}
static void errorCb(int error, const char* description)
{
  die(description);
}

class MainLoop
{
public:
  GLFWwindow *window;
  double simulatedTime;
  MainLoop() {
    simulatedTime = 0;
    if (!glfwInit())
      die("Failed to initialize GLFW");
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    window = glfwCreateWindow(Globals.windowWidth, Globals.windowHeight,
        "klojjeg", NULL, NULL);
    if (!window) {
      glfwTerminate();
      die("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyInputCb);
    glfwSetCursorPosCallback(window, mouseMoveCb);
    glfwSetMouseButtonCallback(window, mouseInputCb);
    glfwSetScrollCallback(window, mouseScrollCb);
    glfwSetErrorCallback(errorCb);
  };
  bool Update() {
    return !glfwWindowShouldClose(window) && !Globals.quit;
  }
  void Display() {
    int fbw, fbh;
    ImGuiIO& io = ImGui::GetIO();
    glfwGetWindowSize(window, &Globals.windowWidth, &Globals.windowHeight);
    glfwGetFramebufferSize(window, &fbw, &fbh);
    io.DisplaySize = ImVec2((float)fbw, (float)fbh);
    io.DisplayFramebufferScale =
      ImVec2((float)fbw/Globals.windowWidth, (float)fbh/Globals.windowHeight);
    ImGui::NewFrame();
    Globals.guiptr->Draw();
    glfwSwapBuffers(window);
  }
  ~MainLoop() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

int main()
{
  MainLoop ml;
  Gui gui;
  Globals.guiptr = &gui;
  ImGuiIO& io = ImGui::GetIO();

  const ImVec4 clearColor = ImColor(200, 200, 200);

  ImFont *imFont;
  std::unique_ptr<char> fontFileBuffer;
  if (!FontLoader::loadEmbeddedFont(imFont, fontFileBuffer,
        _CommeLight_ttf.data, _CommeLight_ttf.size))
    return 1;
  gui.CreateFontTexture(imFont);

  while (ml.Update()) {
    glfwPollEvents();
    double realTime = glfwGetTime();
    while (ml.simulatedTime < realTime) {
      ml.simulatedTime += glfwGetTime();
    }
    for (int i = 0; i < 3; i++) {
      io.MouseDown[i] = Globals.guiptr->mousePressed[i]
        || (glfwGetMouseButton(ml.window, i) != 0);
      Globals.guiptr->mousePressed[i] = false;
    }

    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ml.Display();
  }

  return 0;
}

// vim: et:sw=2

