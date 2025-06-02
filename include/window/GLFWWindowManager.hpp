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
  void run();
  void cleanup();

  GLFWwindow* getWindow() const { return window; }
  void getFramebufferSize(int& width, int& height) const;

 private:
  GLFWwindow* window;
  std::function<void()> renderCallback;

  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};
