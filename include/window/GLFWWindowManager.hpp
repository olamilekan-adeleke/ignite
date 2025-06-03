#pragma once
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

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

 private:
  GLFWwindow* window;
  std::function<void()> renderCallback;
  std::function<void(int, int)> resizeCallback;

  // Static callback functions
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void windowSizeCallback(GLFWwindow* window, int width, int height);
  static void windowRefreshCallback(GLFWwindow* window);
};
