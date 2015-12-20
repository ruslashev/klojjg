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
  io.KeysDown[key] = action == GLFW_PRESS;
}
static void mouseMoveCb(GLFWwindow *window, double xpos, double ypos)
{
  Globals.guiptr->mousePosX = xpos;
  Globals.guiptr->mousePosY = ypos;
}
static void mouseInputCb(GLFWwindow *window, int button, int action, int mods)
{
  Globals.guiptr->mousePressed[button] = action == GLFW_PRESS;
}
static void mouseScrollCb(GLFWwindow *window, double xoffset, double yoffset)
{
  ImGui::GetIO().MouseWheel += yoffset;
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
    glfwGetFramebufferSize(window, &Globals.windowWidth, &Globals.windowHeight);
    glViewport(0, 0, Globals.windowWidth, Globals.windowHeight);
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

  ImFont *imFont;
  std::unique_ptr<char> fontFileBuffer;
  if (!FontLoader::loadEmbeddedFont(imFont, fontFileBuffer,
        _CommeLight_ttf.data, _CommeLight_ttf.size))
    return 1;
  gui.CreateFontTexture(imFont);

  while (ml.Update()) {
    double realTime = glfwGetTime();
    while (ml.simulatedTime < realTime) {
      Globals.guiptr->Update(Constants.updateMilliseconds);
      ml.simulatedTime += glfwGetTime();
    }

    ml.Display();
    glfwPollEvents();
  }

  return 0;
}

// vim: et:sw=2

