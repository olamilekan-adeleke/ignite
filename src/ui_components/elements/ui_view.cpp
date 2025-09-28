#include "elements/ui_view.hpp"
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>
#include "size.hpp"

View::View(const ViewParams &params) : params_(params) { setTapListener(params_.onTap); }

UISize View::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize childSize{0.0f, 0.0f};

  if (params_.child) {
    const auto horizonalSpace = params_.margin.horizonal() + params_.insets.horizonal();
    const auto verticalSpace = params_.margin.vertical() + params_.insets.vertical();

    auto constraintsShrinked = constraints.shrinkBy(horizonalSpace, verticalSpace);
    childSize = params_.child->getIntrinsicSize(constraintsShrinked);
  }

  const float totalWidth = childSize.width + params_.insets.horizonal() + params_.margin.horizonal();
  const float totalHeight = childSize.height + params_.insets.vertical() + params_.margin.vertical();

  return UISize{totalWidth, totalHeight};
}

void View::layout(UISize size) {
  const bool wantsToFillMainAxis = this->wantsToFillMainAxis();

  if (params_.child) {
    // Calculate available space after removing margins
    float availableWidth = size.width - params_.margin.horizonal();
    float availableHeight = size.height - params_.margin.vertical();

    // Calculate child space after removing insets
    float availableChildWidth = std::max(0.0f, availableWidth - params_.insets.horizonal());
    float availableChildHeight = std::max(0.0f, availableHeight - params_.insets.vertical());

    // Layout child with the correct constrained size
    params_.child->layout({availableChildWidth, availableChildHeight});

    // Get child's actual bounds after layout
    float childActualWidth = params_.child->getBounds().width;
    float childActualHeight = params_.child->getBounds().height;

    if (wantsToFillMainAxis) {
      // Use the full allocated size minus margins
      bounds_.width = availableWidth;
      bounds_.height = availableHeight;

      // Center child within available space
      float childX =
          params_.margin.left + params_.insets.left + std::max(0.0f, (availableChildWidth - childActualWidth) / 2.0f);
      float childY =
          params_.margin.top + params_.insets.top + std::max(0.0f, (availableChildHeight - childActualHeight) / 2.0f);

      params_.child->setPosition(childX, childY);
    } else {
      // Size to content plus padding
      params_.child->setPosition(params_.margin.left + params_.insets.left, params_.margin.top + params_.insets.top);
      bounds_.width = childActualWidth + params_.insets.horizonal() + params_.margin.horizonal();
      bounds_.height = childActualHeight + params_.insets.vertical() + params_.margin.vertical();
    }
  } else {
    // No child case
    if (wantsToFillMainAxis) {
      bounds_.width = size.width - params_.margin.horizonal();
      bounds_.height = size.height - params_.margin.vertical();
    } else {
      bounds_.width = params_.insets.horizonal() + params_.margin.horizonal();
      bounds_.height = params_.insets.vertical() + params_.margin.vertical();
    }
  }
}

void View::draw(SkCanvas *canvas) {
  if (params_.child) {
    // set up paint stuff and all
    SkPaint paint;
    paint.setColor(params_.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(params_.antiAlias);

    // center the background within the margin area
    float backgroundX = bounds_.x + params_.margin.left;
    float backgroundY = bounds_.y + params_.margin.top;
    float backgroundWidth = bounds_.width - params_.margin.horizonal();
    float backgroundHeight = bounds_.height - params_.margin.vertical();

    SkRect rect = SkRect::MakeXYWH(backgroundX, backgroundY, backgroundWidth, backgroundHeight);

    SkRRect rrect;
    rrect.setRectXY(rect, params_.borderRadius, params_.borderRadius);
    if (params_.borderRadius > 0) {
      canvas->drawRRect(rrect, paint);
    } else {
      canvas->drawRect(rect, paint);
    }

    canvas->save();
    if (params_.borderRadius > 0) {
      canvas->clipRRect(rrect, SkClipOp::kIntersect, true);
    } else {
      canvas->clipRect(rect, SkClipOp::kIntersect, true);
    }

    // Position child - they're already positioned correctly in layout()
    canvas->translate(bounds_.x, bounds_.y);
    params_.child->draw(canvas);
    canvas->restore();
  } else {
    if (params_.insets.vertical() > 0 || params_.insets.horizonal() > 0) {
      // set up paint stuff and all
      SkPaint paint;
      paint.setColor(params_.backgroundColor);
      paint.setStyle(SkPaint::kFill_Style);
      paint.setAntiAlias(params_.antiAlias);

      // Calculate available space after margins
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
    }
  }
  UIComponent::draw(canvas);
}

const std::vector<std::shared_ptr<UIComponent>> &View::children() const {
  if (params_.child) return {params_.child};
  static std::vector<std::shared_ptr<UIComponent>> cache;
  return cache;
}
