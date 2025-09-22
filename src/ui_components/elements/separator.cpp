#include "basic/ui_component.hpp"
#include "layout/flex_box.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "ui_manager.hpp"
#include "elements/separator.hpp"
#include <fmt/base.h>
#include <include/core/SkRect.h>
#include <cstdint>

void Separator::markHasDirty(const UIMarkDirtyType& type, const UIMarkDirtyCaller& caller) noexcept {
  const auto layoutHash{params_.layoutHashCode()};
  const auto drawHash{params_.drawHashCode()};

  if (type == UIMarkDirtyType::DRAW) {
    UICacheManager::instance().removeCachedSurface(drawHash);
    return;
  }

  UICacheManager::instance().removeCachedSurface(drawHash);
  UICacheManager::instance().removeLayoutCached(layoutHash);
};

UISize Separator::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize size{0, 0};
  if (params_.axis == Axis::VERTICAL) {
    size = UISize{params_.thinkness, constraints.minHeight};
  } else if (params_.axis == Axis::HORIZONTAL) {
    size = UISize{constraints.minWidth, params_.thinkness};
  }

  const uint64_t layoutHash{params_.layoutHashCode()};
  const std::optional<UIRect> cacheIt = UICacheManager::instance().getLayoutCached(layoutHash);

  if (cacheIt.has_value() && size != UISize{.width = bounds_.width, .height = bounds_.height}) {
    markHasDirty(UIMarkDirtyType::LAYOUT, UIMarkDirtyCaller::NONE);
  }

  return size;
}

void Separator::layout(UISize size) {
  const uint64_t layoutHash{params_.layoutHashCode()};
  const auto cacheIt = UICacheManager::instance().getLayoutCached(layoutHash);
  if (cacheIt) {
    setSize(cacheIt->width, cacheIt->height);
    return;
  }

  if (params_.axis == Axis::VERTICAL) {
    setSize(params_.thinkness, size.height);
  } else if (params_.axis == Axis::HORIZONTAL) {
    setSize(size.width, params_.thinkness);
  }

  UICacheManager::instance().setLayoutCached(layoutHash, bounds_);
  markHasDirty(UIMarkDirtyType::DRAW, UIMarkDirtyCaller::NONE);
}

void Separator::draw(SkCanvas* canvas) {
  const uint64_t drawHash{params_.drawHashCode()};
  const auto cachedSurface = UICacheManager::instance().getCachedSurface(drawHash);
  if (cachedSurface) {
    canvas->drawImage(cachedSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
    return;
  }

  SkPaint paint;
  paint.setColor(params_.backgroundColor);
  paint.setStyle(SkPaint::kFill_Style);

  SkImageInfo info = SkImageInfo::Make(bounds_.width, bounds_.height, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
  sk_sp<SkSurface> newSurface = SkSurfaces::Raster(info);
  if (!newSurface) {
    canvas->drawRect(bounds_.toSkRect(), paint);
    return;
  }

  SkCanvas* surfaceCanvas = newSurface->getCanvas();
  surfaceCanvas->drawRect(bounds_.toSkRectHW(), paint);

  sk_sp<SkImage> img = newSurface->makeImageSnapshot();
  if (!img) {
    canvas->drawRect(bounds_.toSkRect(), paint);
    return;
  }

  UICacheManager::instance().setCachedSurface(drawHash, newSurface);
  canvas->drawImage(newSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
}
