#pragma once

#include "basic/ui_component.hpp"

enum class ScrollDirection { Vertical, Horizontal };

struct ScrollViewParam {
  std::shared_ptr<UIComponent> child;
  ScrollDirection direction = ScrollDirection::Vertical;
};

class ScrollView : public UIComponent {
 public:
  ScrollView(const ScrollViewParam& param = {}) : params_(param) { setShouldHandleHover(true); }

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  const std::vector<std::shared_ptr<UIComponent>>& children() const override;

  // bool wantsToFillMainAxis() const override { return true; }
  // bool wantsToFillCrossAxis() const override { return true; }

 protected:
  void update();

  void onHoverMove(Offset& mousePosition) noexcept override;
  void setCursorPosCallback(Offset offset) noexcept override;

  bool processChildTaps(const UITapEvent& event) override {
    if (params_.child) {
      UITapEvent localEvent = event;
      localEvent.x += scrollOffset_.x;
      localEvent.y += scrollOffset_.y;

      return params_.child->processTap(localEvent);
    }

    return false;
  }

 private:
  Offset scrollOffset_{0, 0};
  Offset targetScrollOffset_{0, 0};
  ScrollViewParam params_;
  float scrollSpeed_ = 2.0f;
  float smoothness_ = 0.2f;
};

inline void ScrollView::update() {
  scrollOffset_.x += (targetScrollOffset_.x - scrollOffset_.x) * smoothness_;
  scrollOffset_.y += (targetScrollOffset_.y - scrollOffset_.y) * smoothness_;
}

inline void ScrollView::onHoverMove(Offset& mousePosition) noexcept {
  if (!this->shouldHandleHover()) return;

  UIRect boundsUIRect{getGobalBounds()};
  if (hitTest(mousePosition, boundsUIRect)) {
    onHoverEnter();
  } else {
    onHoverExit();
  }
}

inline void ScrollView::setCursorPosCallback(Offset offset) noexcept {
  if (!this->shouldHandleHover()) return;

  if (params_.direction == ScrollDirection::Vertical) {
    float delta = offset.y * scrollSpeed_;
#if defined(SK_BUILD_FOR_MAC)
    delta = -delta;
#endif
    targetScrollOffset_.y += delta;

  } else if (params_.direction == ScrollDirection::Horizontal) {
    float delta = offset.x * scrollSpeed_;
#if defined(SK_BUILD_FOR_MAC)
    delta = -delta;
#endif
    targetScrollOffset_.x += delta;
  }
}
