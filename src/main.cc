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

class MainLoop
{
public:
  GLFWwindow *window;
  double simulatedTime;
  MainLoop() {
    if (!glfwInit())
      die("Failed to initialize GLFW");
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
  };
  bool Update() {
    Globals.guiptr->Update(Constants.updateMilliseconds);
    simulatedTime += glfwGetTime();
    return !glfwWindowShouldClose(window) && !Globals.quit;
  }
  void Display() {
    Globals.guiptr->Draw();
    glfwSwapBuffers(window);
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
      glfwPollEvents();
      ml.Update();
    }

    ImGui::NewFrame();

    ml.Display();
  }

  return 0;
}

// vim: et:sw=2

