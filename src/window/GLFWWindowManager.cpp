#include "window/GLFWWindowManager.hpp"

#include <iostream>

#include "tap_event.hpp"
#include "window/GLFWWindowManager.hpp"

GLFWWindowManager::GLFWWindowManager() : window(nullptr) {}

GLFWWindowManager::~GLFWWindowManager() { cleanup(); }

bool GLFWWindowManager::initialize(int width, int height, const std::string &title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // Enable proper DPI scaling
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

  // // Make window non-resizable
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

// Enable continuous refresh during resize (Windows specific)
#ifdef _WIN32
  glfwWindowHint(GLFW_WIN32_KEYBOARD_MENU, GLFW_FALSE);
#endif

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  
  // Get current content scale
  // glfwSetWindowContentScaleCallback(window, NULL);
  // glfwGetWindowContentScale(window, 1.0f, 1.0f);
  float xscale, yscale;
  glfwGetWindowContentScale(window, &xscale, &yscale);
  
  // Log the current content scale for debugging
  std::cout << "Window content scale: " << xscale << ", " << yscale << std::endl;

  // Set up mouse callbacks
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, mouseMoveCallback);
  glfwSetScrollCallback(window, scrollCallback);

  // Enable cursor position events
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Set up callbacks for real-time resizing
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
  glfwSetWindowRefreshCallback(window, windowRefreshCallback);

  // Enable vsync for smoother rendering
  glfwSwapInterval(1);

  return true;
}

void GLFWWindowManager::setRenderCallback(std::function<void()> callback) { renderCallback = callback; }

void GLFWWindowManager::setResizeCallback(std::function<void(int, int)> callback) { resizeCallback = callback; }

void GLFWWindowManager::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwWaitEventsTimeout(0.016);

    if (renderCallback) {
      renderCallback();
    }

    glfwSwapBuffers(window);
  }
}

void GLFWWindowManager::getFramebufferSize(int &width, int &height) const {
  glfwGetFramebufferSize(window, &width, &height);
}

void GLFWWindowManager::cleanup() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

// Static callback functions
void GLFWWindowManager::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // Update OpenGL viewport immediately
  glViewport(0, 0, width, height);

  // Get the window manager instance and trigger custom resize logic
  GLFWWindowManager *manager = static_cast<GLFWWindowManager *>(glfwGetWindowUserPointer(window));
  if (manager && manager->resizeCallback) {
    manager->resizeCallback(width, height);
  }

  // Force a render during resize for real-time updates
  if (manager && manager->renderCallback) {
    manager->renderCallback();
    glfwSwapBuffers(window);
  }
}

void GLFWWindowManager::windowSizeCallback(GLFWwindow *window, int width, int height) {
  // This callback is called when the window content area is resized
  GLFWWindowManager *manager = static_cast<GLFWWindowManager *>(glfwGetWindowUserPointer(window));

  // Immediately render during resize
  if (manager && manager->renderCallback) {
    manager->renderCallback();
    glfwSwapBuffers(window);
  }
}

void GLFWWindowManager::windowRefreshCallback(GLFWwindow *window) {
  // This callback is called when the window needs to be redrawn
  GLFWWindowManager *manager = static_cast<GLFWWindowManager *>(glfwGetWindowUserPointer(window));

  if (manager && manager->renderCallback) {
    manager->renderCallback();
    glfwSwapBuffers(window);
  }
}
