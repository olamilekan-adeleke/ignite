#include "elements/ui_view.hpp"

#include <fmt/base.h>
#include <include/core/SkPaint.h>
#include <include/core/SkRRect.h>
#include <include/core/SkRect.h>

#include <algorithm>

#include "foundation/foundation.hpp"
#include "foundation/geometry/rect.hpp"

void ViewRenderObject::performLayout(UIConstraints size) noexcept {
  const bool wantsToFillMainAxis = this->wantsToFillMainAxis();

  const auto& insets = params_.insets;
  const float horizonalSpace = insets.horizonal();
  const float verticalSpace = insets.vertical();

  const auto& child = children_.size() ? children_.front() : nullptr;

  if (child) {
    float availableChildWidth = std::max(0.0f, size.maxWidth - horizonalSpace);
    float availableChildHeight = std::max(0.0f, size.maxHeight - verticalSpace);

    child->performLayout(UIConstraints::maxSize(availableChildWidth, availableChildHeight));
    const auto childSize = child->getSize();

    float finalWidth = std::clamp(childSize.width + horizonalSpace, size.minWidth, size.maxWidth);
    float finalHeight = std::clamp(childSize.height + verticalSpace, size.minHeight, size.maxHeight);
    setSize(finalWidth, finalHeight);

    float childX = insets.left;
    float childY = insets.top;
    child->setPosition(childX, childY);
    // child->updateGlobalOffset({getGlobalOffset().x + childX, getGlobalOffset().y + childY});
  } else {
    if (wantsToFillMainAxis) {
      setSize(size.width, size.height);
    } else {
      setSize(horizonalSpace, verticalSpace);
    }
  }

  // fmt::println(
  //     "ViewRenderObject::performLayout {} {} | has child", bounds_.width, bounds_.height, child ? "true" : "false");
}

void ViewRenderObject::paint(SkCanvas* canvas) noexcept {
  const auto& child = children_.size() ? children_.front() : nullptr;
  const UIRect bounds_{getBounds()};

  if (child) {
    // set up paint stuff and all
    SkPaint paint;
    paint.setColor(params_.backgroundColor);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(params_.antiAlias);

    // center the background within the margin area
    float backgroundX = bounds_.x;
    float backgroundY = bounds_.y;
    float backgroundWidth = bounds_.width;
    float backgroundHeight = bounds_.height;

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
    child->paint(canvas);
    canvas->restore();
  } else {
    if (params_.insets.vertical() > 0 || params_.insets.horizonal() > 0) {
      // set up paint stuff and all
      SkPaint paint;
      paint.setColor(params_.backgroundColor);
      paint.setStyle(SkPaint::kFill_Style);
      paint.setAntiAlias(params_.antiAlias);

      float backgroundX = bounds_.x;
      float backgroundY = bounds_.y;
      float backgroundWidth = bounds_.width;
      float backgroundHeight = bounds_.height;

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

  RenderObject::paint(canvas);
}
