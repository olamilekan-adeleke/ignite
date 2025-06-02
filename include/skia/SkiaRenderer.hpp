
#pragma once
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"

// For GPU context - try these in order based on your Skia version
// #include "include/gpu/GrDirectContext.h"
// #include "include/gpu/gl/GrGLInterface.h"
// Older Skia versions (before M87)
// #include "include/gpu/GrContext.h"
// #include "include/gpu/gl/GrGLInterface.h"
// #endif

// include GrContext
#include "include/core/SkSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLConfig.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"

class SkiaRenderer {
 public:
  SkiaRenderer();
  ~SkiaRenderer();

  bool initialize(int width, int height);
  void resize(int width, int height);
  void beginFrame();
  void endFrame();
  void cleanup();

  SkCanvas* getCanvas() const { return canvas; }

 private:
  sk_sp<GrDirectContext> grContext;
  SkSurface* surface = nullptr;
  SkCanvas* canvas = nullptr;
  int width, height;

  void createSurface();
};
