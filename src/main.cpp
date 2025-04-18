#include <cstdlib>
#define GL_SILENCE_DEPRECATION

#include "GLFW/glfw3.h"
#include "core/logger.hpp"

int screenWidth = 800;
int screenHeight = 600;

int main(int argc, char *argv[]) {
  if (!glfwInit()) {
    Logger::log("Failed to initialize GLFW");
    return EXIT_FAILURE;
  }

  // Set GLFW window hints (optional, but recommended)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "Ignite", nullptr, nullptr);
  if (!window) {
    Logger::log("Failed to create GLFW window");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set clear color (dark cyan)
    glClear(GL_COLOR_BUFFER_BIT);          // Clear the color buffer

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
