#include "ui_view.hpp"

#include <fmt/base.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>

View::View(const ViewParams &params) : params_(params) {}

void View::layout(float parentWidth, float parentHeight) {
  // bounds_.width = parentWidth;
  // bounds_.height = parentHeight;
  //
  fmt::println("View parentWidth: {} X parentHeight: {}", parentWidth,
               parentWidth);

  if (params_.child) {
    float availableWidth =
        parentWidth - (params_.insets.left + params_.insets.right);
    float availableHeight =
        parentHeight - (params_.insets.top + params_.insets.bottom);

    // TODO: Constrain to non-negative (might want to remove later)
    if (availableWidth < 0)
      availableWidth = 0;
    if (availableHeight < 0)
      availableHeight = 0;

    params_.child->layout(availableWidth, availableHeight);
    params_.child->setPosition(params_.insets.left, params_.insets.top);

    bounds_.width = params_.child->getBounds().width + params_.insets.left +
                    params_.insets.right;
    bounds_.height = params_.child->getBounds().height + params_.insets.top +
                     params_.insets.bottom;

  } else {
    bounds_.width = params_.insets.left + params_.insets.right;
    bounds_.height = params_.insets.top + params_.insets.bottom;
  }
}

void View::draw(SkCanvas *canvas) {
  SkPaint paint;
  paint.setColor(params_.backgroundColor);
  paint.setStyle(SkPaint::kFill_Style);
  paint.setAntiAlias(params_.antiAlias);

  // canvas->save();

  SkRect rect =
      SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  if (params_.borderRadius > 0) {
    SkRRect rrect;
    rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
    canvas->drawRRect(rrect, paint);
  } else {
    canvas->drawRect(rect, paint);
  }

  // canvas->restore();

  if (params_.child) {
    canvas->save();
    canvas->translate(bounds_.x, bounds_.y);
    params_.child->draw(canvas);
    canvas->restore();
  }
}
