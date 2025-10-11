#include "elements/checkbox_render.hpp"

#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkRRect.h>

#include <cstdint>

#include "basic/ui_component.hpp"
#include "size.hpp"
#include "ui_manager.hpp"

UISize CheckBoxRender::getIntrinsicSize(UIConstraints constraints) noexcept { return params_.size; }

void CheckBoxRender::markHasDirty(const UIMarkDirtyType &type, const UIMarkDirtyCaller &caller) noexcept {
  const uint64_t drawHash{params_.drawHashCode()};

  if (type == UIMarkDirtyType::DRAW) {
    UICacheManager::instance().removeCachedSurface(drawHash);
    return;
  }
}

void CheckBoxRender::layout(UIConstraints size) {
  setSize(params_.size.width, params_.size.height);
  // setSize(size.width, size.height);
}

void CheckBoxRender::draw(SkCanvas *canvas) {
  const uint64_t drawHash{params_.drawHashCode()};
  const sk_sp<SkSurface> cacheSurface = UICacheManager::instance().getCachedSurface(drawHash);
  if (cacheSurface) {
    canvas->drawImage(cacheSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
    UIComponent::draw(canvas);
    return;
  }

  const int w = static_cast<int>(bounds_.width);
  const int h = static_cast<int>(bounds_.height);

  SkImageInfo info = SkImageInfo::Make(w, h, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
  sk_sp<SkSurface> surface = SkSurfaces::Raster(info);

  auto drawBoxAndCheck = [&](SkCanvas *canvas) {
    SkPaint boxPaint;
    boxPaint.setAntiAlias(true);
    boxPaint.setColor(params_.checked ? params_.fillColor : params_.inactiveBorderColor);
    boxPaint.setStyle(params_.checked ? SkPaint::kFill_Style : SkPaint::kStroke_Style);
    if (!params_.checked) boxPaint.setStrokeWidth(1.0f);

    SkRRect rrect;
    rrect.setRectXY(SkRect::MakeWH(static_cast<SkScalar>(w), static_cast<SkScalar>(h)), params_.radius, params_.radius);

    canvas->drawRRect(rrect, boxPaint);
    if (params_.checked) drawCheckBox(canvas);
  };

  if (surface) {
    SkCanvas *surfaceCanvas = surface->getCanvas();
    drawBoxAndCheck(surfaceCanvas);
    UICacheManager::instance().setCachedSurface(drawHash, surface);
    canvas->drawImage(surface->makeImageSnapshot(), bounds_.x, bounds_.y);
  } else {
    SkAutoCanvasRestore acr(canvas, true);
    canvas->translate(bounds_.x, bounds_.y);
    drawBoxAndCheck(canvas);
  }

  UIComponent::draw(canvas);
}

void CheckBoxRender::drawCheckBox(SkCanvas *canvas) const {
  SkPaint strokePaint;
  strokePaint.setColor(params_.checkColor);
  strokePaint.setAntiAlias(true);
  strokePaint.setStyle(SkPaint::kStroke_Style);
  strokePaint.setStrokeCap(SkPaint::kRound_Cap);
  strokePaint.setStrokeWidth(2.0f);

  SkPath checkPath;
  const SkScalar w = static_cast<SkScalar>(bounds_.width);
  const SkScalar h = static_cast<SkScalar>(bounds_.height);

  checkPath.moveTo(w * 0.2, h * 0.5);
  checkPath.lineTo(w * 0.4, h * 0.7);
  checkPath.lineTo(w * 0.8, h * 0.3);

  canvas->drawPath(checkPath, strokePaint);
}
