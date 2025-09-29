#pragma once
#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include "basic/ui_hover_handler.hpp"
#include "basic/ui_text_input_handler.hpp"

class GLFWWindowManager {
 public:
  GLFWWindowManager();
  ~GLFWWindowManager();

  bool initialize(int width, int height, const std::string& title);
  void setRenderCallback(std::function<void()> callback);
  void setResizeCallback(std::function<void(int, int)> callback);
  void run();
  void getFramebufferSize(int& width, int& height) const;
  void cleanup();

  void setKeyCallback(KeyCallback callback);
  void setCursorPosCallback(CursorPosCallback callback);
  void setCharCallback(CharCallback callback);

 private:
  GLFWwindow* window;
  std::function<void()> renderCallback;
  std::function<void(int, int)> resizeCallback;

  KeyCallback keyCallback;
  CursorPosCallback cursorPosCallback;
  CharCallback charCallback;

  // Static callback functions
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void windowSizeCallback(GLFWwindow* window, int width, int height);
  static void windowRefreshCallback(GLFWwindow* window);

  static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void staticCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  static void staticCharCallback(GLFWwindow* window, unsigned int codepoint);
};
