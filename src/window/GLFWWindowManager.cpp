#include "window/GLFWWindowManager.hpp"

#include <iostream>

GLFWWindowManager::GLFWWindowManager() : window(nullptr) {}

GLFWWindowManager::~GLFWWindowManager() { cleanup(); }

bool GLFWWindowManager::initialize(int width, int height, const std::string& title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  return true;
}

void GLFWWindowManager::setRenderCallback(std::function<void()> callback) { renderCallback = callback; }

void GLFWWindowManager::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    if (renderCallback) {
      renderCallback();
    }

    glfwSwapBuffers(window);
  }
}

void GLFWWindowManager::getFramebufferSize(int& width, int& height) const {
  glfwGetFramebufferSize(window, &width, &height);
}

void GLFWWindowManager::cleanup() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void GLFWWindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
