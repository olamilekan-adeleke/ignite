#include "elements/checkbox_render.hpp"

#include <include/core/SkPath.h>
#include <include/core/SkRRect.h>

#include "foundation/foundation.hpp"
#include "ui_manager.hpp"

void CheckBoxRender::performLayout(UIConstraints constraints) noexcept {
  const float width = std::clamp(params_.size.width, 0.0f, constraints.maxWidth);
  const float height = std::clamp(params_.size.height, 0.0f, constraints.maxHeight);
  setSize(width, height);
}

void CheckBoxRender::paint(SkCanvas *canvas) noexcept {
  const UIRect bounds_{getBounds()};

  const int w = static_cast<int>(bounds_.width);
  const int h = static_cast<int>(bounds_.height);

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

  SkAutoCanvasRestore acr(canvas, true);
  canvas->translate(bounds_.x, bounds_.y);
  drawBoxAndCheck(canvas);

  RenderObject::paint(canvas);
}

void CheckBoxRender::drawCheckBox(SkCanvas *canvas) const {
  SkPaint strokePaint;
  strokePaint.setColor(params_.checkColor);
  strokePaint.setAntiAlias(true);
  strokePaint.setStyle(SkPaint::kStroke_Style);
  strokePaint.setStrokeCap(SkPaint::kRound_Cap);
  strokePaint.setStrokeWidth(2.0f);

  SkPath checkPath;
  const UIRect bounds_{getBounds()};
  const SkScalar w = static_cast<SkScalar>(bounds_.width);
  const SkScalar h = static_cast<SkScalar>(bounds_.height);

  checkPath.moveTo(w * 0.2, h * 0.5);
  checkPath.lineTo(w * 0.4, h * 0.7);
  checkPath.lineTo(w * 0.8, h * 0.3);

  canvas->drawPath(checkPath, strokePaint);
}
