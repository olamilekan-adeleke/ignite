#include "../include/skia/SkiaRenderer.hpp"

#include <include/core/SkColorSpace.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/gpu/ganesh/gl/GrGLBackendSurface.h>
#include <include/gpu/ganesh/gl/GrGLDirectContext.h>

#include "include/core/SkSurface.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "logger.hpp"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <iostream>

SkiaRenderer::SkiaRenderer() : canvas(nullptr), width(0), height(0) {}

SkiaRenderer::~SkiaRenderer() { cleanup(); }

bool SkiaRenderer::initialize(int w, int h) {
  width = w;
  height = h;

  // Create OpenGL context for Skia
  auto glInterface = GrGLMakeNativeInterface();
  if (!glInterface) {
    std::cerr << "Failed to create GL interface" << std::endl;
    return false;
  }

  sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
  grContext = GrDirectContexts::MakeGL(interface);

  if (!grContext) {
    std::cerr << "Failed to create GrDirectContext" << std::endl;
    return false;
  }

  Logger::log("GoThroughSkia");
  createSurface();
  return canvas != nullptr;
}

void SkiaRenderer::resize(int w, int h) {
  if (w != width || h != height) {
    width = w;
    height = h;
    createSurface();
  }
}

void SkiaRenderer::createSurface() {
  if (!grContext) return;

  Logger::log("createSurface line 63");
  GrGLint buffer;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);

  GrGLFramebufferInfo framebufferInfo;
  framebufferInfo.fFBOID = buffer;
  framebufferInfo.fFormat = GL_RGBA8;
  SkColorType colorType = kRGBA_8888_SkColorType;

  GrBackendRenderTarget backendRenderTarget = GrBackendRenderTargets::MakeGL(width, height,
                                                                             0,  // sample count
                                                                             0,  // stencil bits
                                                                             framebufferInfo);

  if (surface) {
    surface->unref();
    surface = nullptr;
  }

  surface = SkSurfaces::WrapBackendRenderTarget(grContext.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin,
                                                colorType, SkColorSpace::MakeSRGB(), nullptr)
                .release();

  if (surface) {
    canvas = surface->getCanvas();
    Logger::log("Created Skia surface");
  } else {
    std::cerr << "Failed to create Skia surface" << std::endl;
    canvas = nullptr;
  }
}

void SkiaRenderer::beginFrame() {
  if (canvas) {
    canvas->clear(SK_ColorWHITE);
  }
}

void SkiaRenderer::endFrame() {
  if (surface) {
    grContext->flush();
  }
}

void SkiaRenderer::cleanup() {
  // surface->unref();
  grContext.reset();
  canvas = nullptr;
}
