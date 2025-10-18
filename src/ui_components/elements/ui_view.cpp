#include "elements/ui_view.hpp"

#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>

#include <algorithm>

#include "foundation/foundation.hpp"

void View::layout(UIConstraints size) {
  const bool wantsToFillMainAxis = this->wantsToFillMainAxis();

  const auto& margin = params_.margin;
  const auto& insets = params_.insets;
  const float horizonalSpace = margin.horizonal() + insets.horizonal();
  const float verticalSpace = margin.vertical() + insets.vertical();

  const auto& child = params_.child;
  if (child) {
    float availableChildWidth = std::max(0.0f, size.maxWidth - horizonalSpace);
    float availableChildHeight = std::max(0.0f, size.maxHeight - verticalSpace);

    child->layout(UIConstraints::maxSize(availableChildWidth, availableChildHeight));
    const auto childSize = child->getSize();

    float finalWidth = std::clamp(childSize.width + horizonalSpace, size.minWidth, size.maxWidth);
    float finalHeight = std::clamp(childSize.height + verticalSpace, size.minHeight, size.maxHeight);
    setSize(finalWidth, finalHeight);

    float childX = margin.left + insets.left;
    float childY = margin.top + insets.top;
    params_.child->setPosition(childX, childY);
    params_.child->updateGlobalOffset({getGlobalOffset().x + childX, getGlobalOffset().y + childY});
  } else {
    if (wantsToFillMainAxis) {
      bounds_.width = size.width;
      bounds_.height = size.height;
    } else {
      bounds_.width = horizonalSpace;
      bounds_.height = verticalSpace;
    }
  }

  bounds_.width = std::clamp(bounds_.width, size.minWidth, size.maxWidth);
  bounds_.height = std::clamp(bounds_.height, size.minHeight, size.maxHeight);
}

void View::draw(SkCanvas* canvas) {
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
