#include "basic/ui_component.hpp"
#include "size.hpp"
#include "ui_manager.hpp"
#include "elements/checkbox_render.hpp"
#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkRRect.h>
#include <cstdint>

UISize CheckBoxRender::getIntrinsicSize(UIConstraints constraints) noexcept { return params_.size; }

void CheckBoxRender::markHasDirty(const UIMarkDirtyType &type, const UIMarkDirtyCaller &caller) noexcept {
  const uint64_t drawHash{params_.drawHashCode()};

  if (type == UIMarkDirtyType::DRAW) {
    UICacheManager::instance().removeCachedSurface(drawHash);
    return;
  }
}

void CheckBoxRender::layout(UISize size) { setSize(params_.size.width, params_.size.height); }

void CheckBoxRender::draw(SkCanvas *canvas) {
  const uint64_t drawHash{params_.drawHashCode()};
  const sk_sp<SkSurface> cacheSurface = UICacheManager::instance().getCachedSurface(drawHash);
  if (cacheSurface) {
    canvas->drawImage(cacheSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
    return;
  }

  SkPaint boxPaint;
  boxPaint.setColor(params_.enable ? params_.fillColor : params_.inactiveBorderColor);
  boxPaint.setAntiAlias(true);
  boxPaint.setStyle(params_.enable ? SkPaint::kFill_Style : SkPaint::kStroke_Style);

  SkRRect rrect;
  rrect.setRectXY(bounds_.toSkRect(), params_.radius, params_.radius);

  SkImageInfo info = SkImageInfo::Make(bounds_.width, bounds_.height, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
  sk_sp<SkSurface> newSurface = SkSurfaces::Raster(info);
  if (!newSurface) {
    canvas->drawRRect(rrect, boxPaint);
    if (params_.enable) drawCheckBox(canvas);
    return;
  }

  canvas->drawRRect(rrect, boxPaint);
  if (params_.enable) drawCheckBox(canvas);
  return;

  SkCanvas *surfaceCanvas = newSurface->getCanvas();
  surfaceCanvas->drawRRect(rrect, boxPaint);
  if (params_.enable) drawCheckBox(surfaceCanvas);

  UICacheManager::instance().setCachedSurface(drawHash, newSurface);
  canvas->drawImage(newSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
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
  float w{bounds_.width};
  float h{bounds_.height};

  checkPath.moveTo(w * 0.2, h * 0.5);
  checkPath.lineTo(w * 0.4, h * 0.7);
  checkPath.lineTo(w * 0.8, h * 0.3);

  canvas->translate(bounds_.x, bounds_.y);

  canvas->drawPath(checkPath, strokePaint);
  canvas->restore();
}
