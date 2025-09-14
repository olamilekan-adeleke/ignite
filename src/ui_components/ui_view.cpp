#include <fmt/base.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>

#include "ui_view.hpp"

View::View(const ViewParams &params) : params_(params) {
  setTapListener(params_.onTap);
  // setTapEnabled(params_.tappable);
}

void View::layout(UISize size) {
  if (params_.child) {
    float availableWidth = size.width - params_.margin.horizonal();
    float availableHeight = size.height - params_.margin.vertical();

    float availableChildWidth = std::fmax(availableWidth - params_.insets.horizonal(), 0);
    float availableChildHeight = std::fmax(availableHeight - params_.insets.vertical(), 0);

    params_.child->layout({availableChildWidth, availableChildHeight});
    params_.child->setPosition(params_.margin.left + params_.insets.left, params_.margin.top + params_.insets.top);

    bounds_.width = params_.child->getBounds().width + (params_.insets.horizonal() + params_.margin.horizonal());
    bounds_.height = params_.child->getBounds().height + (params_.insets.vertical() + params_.margin.vertical());
  } else {
    bounds_.width = params_.insets.horizonal() + params_.margin.horizonal();
    bounds_.height = params_.insets.vertical() + params_.margin.vertical();
  }
}

void View::draw(SkCanvas *canvas) {
  if (params_.child) {
    // set up paint stuff and all
    SkPaint paint;
    paint.setColor(params_.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(params_.antiAlias);

    // background will start after margin
    float backgroundX = bounds_.x + params_.margin.left;
    float backgroundY = bounds_.y + params_.margin.top;
    float backgroundWidth = bounds_.width - params_.margin.horizonal();
    float backgroundHeight = bounds_.height - params_.margin.vertical();

    SkRect rect = SkRect::MakeXYWH(backgroundX, backgroundY, backgroundWidth, backgroundHeight);
    if (params_.borderRadius > 0) {
      SkRRect rrect;
      rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
      canvas->drawRRect(rrect, paint);
    } else {
      canvas->drawRect(rect, paint);
    }

    canvas->save();
    canvas->translate(bounds_.x, bounds_.y);
    params_.child->draw(canvas);
    canvas->restore();
  } else {
    if (params_.insets.vertical() > 0 || params_.insets.horizonal()) {
      // set up paint stuff and all
      SkPaint paint;
      paint.setColor(params_.backgroundColor);
      paint.setStyle(SkPaint::kFill_Style);
      paint.setAntiAlias(params_.antiAlias);

      // background will start after margin
      float backgroundX = bounds_.x + params_.insets.left;
      float backgroundY = bounds_.y + params_.insets.top;
      float backgroundWidth = params_.insets.horizonal();
      float backgroundHeight = params_.insets.vertical();

      SkRect rect = SkRect::MakeXYWH(backgroundX, backgroundY, backgroundWidth, backgroundHeight);

      if (params_.borderRadius > 0) {
        SkRRect rrect;
        rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
        canvas->drawRRect(rrect, paint);
      } else {
        canvas->drawRect(rect, paint);
      }
    }
  }

  UIComponent::draw(canvas);
}

const std::vector<std::shared_ptr<UIComponent>> &View::children() const {
  static std::vector<std::shared_ptr<UIComponent>> cache;
  cache.clear();
  cache.push_back(params_.child);
  return cache;
}
