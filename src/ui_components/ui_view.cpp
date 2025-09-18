#include <fmt/base.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>
#include "size.hpp"

#include "ui_view.hpp"

View::View(const ViewParams &params) : params_(params) {
  setTapListener(params_.onTap);
  // setTapEnabled(params_.tappable);
}

UISize View::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize childSize{0.0f, 0.0f};

  if (params_.child) {
    auto constraintsShrinked = constraints.shrinkBy(params_.margin.horizonal() + params_.insets.horizonal(),
                                                    params_.margin.vertical() + params_.insets.vertical());
    childSize = params_.child->getIntrinsicSize(constraintsShrinked);
  }

  const float totalWidth = childSize.width + params_.insets.horizonal() + params_.margin.horizonal();
  const float totalHeight = childSize.height + params_.insets.vertical() + params_.margin.vertical();

  return UISize{totalWidth, totalHeight};
}

void View::layout(UISize size) {
  const bool wantsToFillMainAxis = this->wantsToFillMainAxis();

  if (params_.child) {
    float availableWidth = size.width - params_.margin.horizonal();
    float availableHeight = size.height - params_.margin.vertical();

    float availableChildWidth = std::fmax(availableWidth - params_.insets.horizonal(), 0);
    float availableChildHeight = std::fmax(availableHeight - params_.insets.vertical(), 0);

    params_.child->layout({availableChildWidth, availableChildHeight});

    if (wantsToFillMainAxis) {
      bounds_.width = size.width;
      bounds_.height = size.height;

      // Position child relative to this View's coordinate system (0,0)
      // since we'll translate the canvas by bounds_.x, bounds_.y in draw()
      float childX =
          params_.margin.left + params_.insets.left + (availableChildWidth - params_.child->getBounds().width) / 2.0f;
      float childY =
          params_.margin.top + params_.insets.top + (availableChildHeight - params_.child->getBounds().height) / 2.0f;

      params_.child->setPosition(childX, childY);
    } else {
      // Position child relative to this View's coordinate system
      params_.child->setPosition(params_.margin.left + params_.insets.left, params_.margin.top + params_.insets.top);
      bounds_.width = params_.child->getBounds().width + (params_.insets.horizonal() + params_.margin.horizonal());
      bounds_.height = params_.child->getBounds().height + (params_.insets.vertical() + params_.margin.vertical());
    }
  } else {
    if (wantsToFillMainAxis) {
      bounds_.width = size.width;
      bounds_.height = size.height;
    } else {
      bounds_.width = params_.insets.horizonal() + params_.margin.horizonal();
      bounds_.height = params_.insets.vertical() + params_.margin.vertical();
    }
  }
}

void View::draw(SkCanvas *canvas) {
  // Always draw background if we have a backgroundColor
  SkPaint paint;
  paint.setColor(params_.backgroundColor);
  paint.setStyle(SkPaint::kFill_Style);
  paint.setAntiAlias(params_.antiAlias);

  // Background starts after margin and covers the area minus margins
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

  // Draw child if present
  if (params_.child) {
    canvas->save();
    // Translate to this View's position
    canvas->translate(bounds_.x, bounds_.y);
    // Child draws at its relative position (set in layout())
    params_.child->draw(canvas);
    canvas->restore();
  }

  UIComponent::draw(canvas);
}

// void View::draw(SkCanvas *canvas) {
//   if (params_.child) {
//     // set up paint stuff and all
//     SkPaint paint;
//     paint.setColor(params_.backgroundColor);
//     paint.setStyle(SkPaint::kFill_Style);
//     paint.setAntiAlias(params_.antiAlias);
//
//     // background will start after margin
//     float backgroundX = bounds_.x + params_.margin.left;
//     float backgroundY = bounds_.y + params_.margin.top;
//     float backgroundWidth = bounds_.width - params_.margin.horizonal();
//     float backgroundHeight = bounds_.height - params_.margin.vertical();
//
//     SkRect rect = SkRect::MakeXYWH(backgroundX, backgroundY, backgroundWidth, backgroundHeight);
//     if (params_.borderRadius > 0) {
//       SkRRect rrect;
//       rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
//       canvas->drawRRect(rrect, paint);
//     } else {
//       canvas->drawRect(rect, paint);
//     }
//
//     canvas->save();
//     canvas->translate(bounds_.x, bounds_.y);
//     params_.child->draw(canvas);
//     canvas->restore();
//   } else {
//     if (params_.insets.vertical() > 0 || params_.insets.horizonal()) {
//       // set up paint stuff and all
//       SkPaint paint;
//       paint.setColor(params_.backgroundColor);
//       paint.setStyle(SkPaint::kFill_Style);
//       paint.setAntiAlias(params_.antiAlias);
//
//       // background will start after margin
//       float backgroundX = bounds_.x + params_.insets.left;
//       float backgroundY = bounds_.y + params_.insets.top;
//       float backgroundWidth = params_.insets.horizonal();
//       float backgroundHeight = params_.insets.vertical();
//
//       SkRect rect = SkRect::MakeXYWH(backgroundX, backgroundY, backgroundWidth, backgroundHeight);
//
//       if (params_.borderRadius > 0) {
//         SkRRect rrect;
//         rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
//         canvas->drawRRect(rrect, paint);
//       } else {
//         canvas->drawRect(rect, paint);
//       }
//     }
//   }
//
//   UIComponent::draw(canvas);
// }

const std::vector<std::shared_ptr<UIComponent>> &View::children() const {
  static std::vector<std::shared_ptr<UIComponent>> cache;
  cache.clear();
  cache.push_back(params_.child);
  return cache;
}
