#include "basic/ui_component.hpp"
#include "layout/flex_box.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "ui_manager.hpp"
#include "elements/separator.hpp"
#include <include/core/SkRect.h>

void Separator::markHasDirty(const UIMarkDirtyType& type, const UIMarkDirtyCaller& caller) noexcept {
  const auto layoutHash = params_.layoutHashCode();
  const auto drawHash = params_.drawHashCode();

  if (type == UIMarkDirtyType::DRAW) {
    UICacheManager::instance().removeCachedSurface(drawHash);
    return;
  }

  UICacheManager::instance().removeCachedSurface(drawHash);
  UICacheManager::instance().removeLayoutCached(layoutHash);
};

UISize Separator::getIntrinsicSize(UIConstraints constraints) noexcept {
  if (params_.axis == Axis::VERTICAL) {
    return UISize{params_.thinkness, constraints.minHeight};
  } else if (params_.axis == Axis::HORIZONTAL) {
    return UISize{constraints.minWidth, params_.thinkness};
  }

  return UISize{0, 0};
}

void Separator::layout(UISize size) {
  const auto layoutHash = params_.layoutHashCode();
  UICacheManager::instance().getCachedSurface(layoutHash);

  if (params_.axis == Axis::VERTICAL) {
    bounds_.width = params_.thinkness;
    bounds_.height = size.height;
  } else if (params_.axis == Axis::HORIZONTAL) {
    bounds_.width = size.width;
    bounds_.height = params_.thinkness;
  }

  UICacheManager::instance().setLayoutCached(layoutHash, bounds_);
  markHasDirty(UIMarkDirtyType::DRAW, UIMarkDirtyCaller::NONE);
}

void Separator::draw(SkCanvas* canvas) {
  SkPaint paint;
  paint.setColor(params_.backgroundColor);
  paint.setStyle(SkPaint::kFill_Style);

  SkRect rect = bounds_.toSkRect();
  canvas->drawRect(rect, paint);

  // UIComponent::draw(canvas);
}
