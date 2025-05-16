#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#define GL_SILENCE_DEPRECATION
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "GLFW/glfw3.h"
#include "core/logger.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkData.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypeface.h"
#define SK_GANESH
#define SK_GL
#include <stdio.h>
#include <stdlib.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/gl/GrGLAssembleInterface.h"
#include "include/gpu/gl/GrGLInterface.h"

int screenWidth = 800;
int screenHeight = 600;

void skia_error_handler(const char* msg, ...) {
  va_list args;
  va_start(args, msg);
  fprintf(stderr, "Skia Error: ");
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
}

int main(int argc, char* argv[]) {
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

  GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Ignite", nullptr, nullptr);
  if (!window) {
    Logger::log("Failed to create GLFW window");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  // Initialize Skia's OpenGL interface
  sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
  if (!interface) {
    Logger::log("Failed to create GrGLInterface");
    return EXIT_FAILURE;
  }

  sk_sp<GrDirectContext> grContext = GrDirectContext::MakeGL(interface);
  if (!grContext) {
    Logger::log("Failed to create GrDirectContext");
    return EXIT_FAILURE;
  }

  GrGLFramebufferInfo fbInfo;
  fbInfo.fFBOID = 0;
  fbInfo.fFormat = GL_RGBA8;

  GrBackendRenderTarget backendRenderTarget(screenWidth, screenHeight, 0, 0, fbInfo);
  SkSurfaceProps props;
  sk_sp<SkSurface> surface = SkSurface::MakeFromBackendRenderTarget(
      grContext.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType, nullptr, &props);

  if (!surface) {
    Logger::log("Failed to create SkSurface");
    return EXIT_FAILURE;
  }

  SkCanvas* canvas = surface->getCanvas();

  // Set up font manager
  sk_sp<SkFontMgr> fontMgr = SkFontMgr::RefEmpty();
  if (!fontMgr) {
    Logger::log("Failed to create font manager");
  }
  const char* fontFilePath = "./assets/Font/CircularStd-Medium.ttf";
  sk_sp<SkData> fontData = SkData::MakeFromFileName(fontFilePath);
  if (!fontData) {
    return EXIT_FAILURE;
  }
  sk_sp<SkTypeface> typeface = fontMgr->makeFromData(fontData);
  if (!typeface) {
    return EXIT_FAILURE;
  }

  // 4. Create an SkFont from the SkTypeface
  SkScalar desiredTextSize = 24;
  SkFont font(typeface, desiredTextSize);

  // Now draw the text
  const char* text = "Hello, World!";
  SkPaint paint;
  paint.setColor(SK_ColorBLACK);  // Set desired text color

  canvas->drawString(text, 10, 20, font, paint);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }

  grContext.reset();
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
