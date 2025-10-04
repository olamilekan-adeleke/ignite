#include "elements/ui_view.hpp"

#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>

#include "size.hpp"
#include "ui_alignment.hpp"

UISize View::getIntrinsicSize(UIConstraints constraints) noexcept {
  UISize childSize{0.0f, 0.0f};

  const auto horizonalSpace = params_.margin.horizonal() + params_.insets.horizonal();
  const auto verticalSpace = params_.margin.vertical() + params_.insets.vertical();

  if (params_.child) {
    auto constraintsShrinked = constraints.shrinkBy(horizonalSpace, verticalSpace);
    childSize = params_.child->getIntrinsicSize(constraintsShrinked);
  }

  UISize size;
  if (params_.mainAxisSize == MainAxisSize::FIT) {
    size.width = childSize.width + horizonalSpace;
    size.height = childSize.height + verticalSpace;
  } else if (params_.mainAxisSize == MainAxisSize::FILL) {
    size.width = 0 + horizonalSpace;
    size.height = 0 + verticalSpace;
  }

  return size;
}

void View::layout(UISize size) {
  const bool wantsToFillMainAxis = this->wantsToFillMainAxis();

  const float horizonalSpace = params_.margin.horizonal() + params_.insets.horizonal();
  const float verticalSpace = params_.margin.vertical() + params_.insets.vertical();

  if (params_.child) {
    float availableChildWidth = std::max(0.0f, size.width - horizonalSpace);
    float availableChildHeight = std::max(0.0f, size.height - verticalSpace);

    const auto& childIntrinsicSize = params_.child->getIntrinsicSize({availableChildWidth, availableChildHeight});
    params_.child->layout({childIntrinsicSize.width, childIntrinsicSize.height});

    float childActualWidth = params_.child->getBounds().width;
    float childActualHeight = params_.child->getBounds().height;

    if (wantsToFillMainAxis) {
      bounds_.width = size.width;
      bounds_.height = size.height;

      float childX =
          params_.margin.left + params_.insets.left + std::max(0.0f, (availableChildWidth - childActualWidth) / 2.0f);
      float childY =
          params_.margin.top + params_.insets.top + std::max(0.0f, (availableChildHeight - childActualHeight) / 2.0f);
      params_.child->setPosition(childX, childY);
    } else {
      bounds_.width = childActualWidth + horizonalSpace;
      bounds_.height = childActualHeight + verticalSpace;

      params_.child->setPosition(params_.margin.left + params_.insets.left, params_.margin.top + params_.insets.top);
    }

    params_.child->updateGlobalOffset(getGlobalOffset());
  } else {
    if (wantsToFillMainAxis) {
      bounds_.width = size.width;
      bounds_.height = size.height;
    } else {
      bounds_.width = horizonalSpace;
      bounds_.height = verticalSpace;
    }
  }
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
