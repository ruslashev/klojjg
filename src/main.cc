#include "constants.hh"
#include "gfx/font.hh"
#include "gfx/fontloader.hh"
#include "gfx/gui.hh"
#include "state.hh"
#include "state_mainmenu.hh"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

static void keyInputCb(GLFWwindow*, int key, int scancode, int action, int mods)
{
  Globals.stateDispatcherPtr->currentState->onKeyInput(key, scancode, action,
      mods);
}
static void mouseMoveCb(GLFWwindow *window, double xpos, double ypos)
{
  if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
    Globals.mousePosX = xpos;
    Globals.mousePosY = ypos;
  }
}
static void mouseInputCb(GLFWwindow*, int button, int action, int)
{
  if (action == GLFW_PRESS && button >= 0 && button <= 3)
    Globals.mousePressed[button] = true;
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
        "klojjg", NULL, NULL);
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
    glfwSwapBuffers(window);
  }
  ~MainLoop() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
};

int main()
{
  try {
    MainLoop ml;
    Gui gui;
    StateDispatcher stateDispatcher;
    stateDispatcher.ChangeTo(&state_mainmenu);

    Globals.guiptr = &gui;
    Globals.stateDispatcherPtr = &stateDispatcher;
    Globals.mousePosX = Globals.mousePosY = 0;
    Globals.mousePressed[0] = Globals.mousePressed[1] =
      Globals.mousePressed[2] = false;
    ImGuiIO& io = ImGui::GetIO();

    ImFont *imFont;
    std::unique_ptr<char> fontFileBuffer;
    if (!FontLoader::loadEmbeddedFont(imFont, fontFileBuffer,
          _CommeLight_ttf.data, _CommeLight_ttf.size))
      return 1;
    gui.CreateFontTexture(imFont);

    while (ml.Update()) {
      glfwPollEvents();

      double realTime = glfwGetTime() * 1000.0;
      while (ml.simulatedTime < realTime) {
        ml.simulatedTime += Constants.updateMilliseconds;
        stateDispatcher.currentState->Update(Constants.updateMilliseconds,
            ml.simulatedTime);
      }

      stateDispatcher.currentState->Draw();

      ml.Display();

      io.MousePos = ImVec2(Globals.mousePosX, Globals.mousePosY);
      for (int i = 0; i < 3; i++) {
        io.MouseDown[i] = Globals.mousePressed[i];
        Globals.mousePressed[i] = false;
      }
    }
  } catch (std::bad_alloc &e) {
    fputs("out of memory\n", stderr);
    return 1;
  } catch (std::runtime_error &e) {
    fputs("die: ", stderr);
    fputs(e.what(), stderr);
    return 1;
  } catch (std::exception &e) {
    fputs("uncaught exception: ", stderr);
    return 1;
  }

  return 0;
}

// vim: et:sw=2

