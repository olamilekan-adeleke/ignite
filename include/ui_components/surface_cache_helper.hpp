#pragma once
#include <functional>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "ui_manager.hpp"

/**
 * Helper class to automatically handle surface caching for UI components.
 * Reduces boilerplate by managing cache lookups, surface creation, and storage.
 */
class SurfaceCacheHelper {
 public:
  /**
   * Draws with automatic caching.
   *
   * @param canvas The target canvas to draw on
   * @param drawHash Unique hash for this draw operation (for cache lookup)
   * @param bounds The bounds where content should be drawn (x, y, width, height)
   * @param drawFunc Lambda that performs the actual drawing on the surface canvas
   * @param alphaType Alpha type for the surface (default: opaque)
   * @return true if drawing was successful, false otherwise
   */
  static bool drawCached(SkCanvas* canvas,
                         uint64_t drawHash,
                         const SkRect& bounds,
                         std::function<void(SkCanvas*)> drawFunc,
                         SkAlphaType alphaType = kOpaque_SkAlphaType) {
    // Try to get cached surface
    sk_sp<SkSurface> cachedSurface = UICacheManager::instance().getCachedSurface(drawHash);
    if (cachedSurface) {
      canvas->drawImage(cachedSurface->makeImageSnapshot(), bounds.x(), bounds.y());
      return true;
    }

    // Create new surface
    SkImageInfo info = SkImageInfo::Make(bounds.width(), bounds.height(), kRGBA_8888_SkColorType, alphaType);
    sk_sp<SkSurface> newSurface = SkSurfaces::Raster(info);
    if (!newSurface) return false;

    // Let caller draw on the surface
    SkCanvas* surfaceCanvas = newSurface->getCanvas();
    drawFunc(surfaceCanvas);

    // Cache and draw
    UICacheManager::instance().setCachedSurface(drawHash, newSurface);
    canvas->drawImage(newSurface->makeImageSnapshot(), bounds.x(), bounds.y());
    return true;
  }

  /**
   * Draws with automatic caching and clipping.
   * Useful when you need to draw directly on the main canvas with clipping.
   *
   * @param canvas The target canvas to draw on
   * @param drawHash Unique hash for this draw operation
   * @param bounds The bounds for clipping and positioning
   * @param drawFunc Lambda that draws on the main canvas (not a surface)
   * @return true if drawing was successful
   */
  static bool drawCachedWithClip(SkCanvas* canvas,
                                 uint64_t drawHash,
                                 const SkRect& bounds,
                                 std::function<void(SkCanvas*)> drawFunc) {
    // Try to get cached surface
    sk_sp<SkSurface> cachedSurface = UICacheManager::instance().getCachedSurface(drawHash);
    if (cachedSurface) {
      canvas->drawImage(cachedSurface->makeImageSnapshot(), bounds.x(), bounds.y());
      return true;
    }

    // Create new surface for caching
    SkImageInfo info = SkImageInfo::Make(bounds.width(), bounds.height(), kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
    sk_sp<SkSurface> newSurface = SkSurfaces::Raster(info);
    if (!newSurface) return false;

    // Draw with clipping
    canvas->save();
    SkRect clipRect = SkRect::MakeXYWH(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    canvas->clipRect(clipRect);
    drawFunc(canvas);
    canvas->restore();

    return true;
  }
};
