#pragma once

#include "component/state_base_component.hpp"
#include "render/render_object.hpp"

enum class ScrollDirection { Vertical, Horizontal };

struct ScrollViewParam {
  ScrollDirection direction = ScrollDirection::Vertical;
};

class ScrollViewRenderObject : public RenderObject {
 public:
  ScrollViewRenderObject(const ScrollViewParam& param = {}) : params_(param) { setShouldHandleHover(true); }

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;

  void updateParams(const ScrollViewParam& param) noexcept { params_ = param; }

 protected:
  void update() {
    scrollOffset_.x += (targetScrollOffset_.x - scrollOffset_.x) * smoothness_;
    scrollOffset_.y += (targetScrollOffset_.y - scrollOffset_.y) * smoothness_;
  }

  void onHoverMove(Offset& mousePosition) noexcept override {
    if (!this->shouldHandleHover()) return;
    UIRect boundsUIRect{getBounds()};
    bool isInside = hitTest(mousePosition, boundsUIRect);

    if (hitTest(mousePosition, boundsUIRect)) {
      if (!isHovered()) onHoverEnter();
      fmt::println("onHoverEnter");
    } else {
      if (isHovered()) onHoverExit();
    }
  }

  void onHoverExit() noexcept override {
    fmt::println("ScrollView::onHoverExit");
    setIsHovered(false);
  }

  void setCursorPosCallback(Offset offset) noexcept override {
    if (!isHovered() || !this->shouldHandleHover()) return;
    fmt::println("setCursorPosCallback: {}", isHovered());

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

  bool processChildTaps(const UITapEvent& event) override {
    if (children_.empty()) return false;
    const auto& child = this->children_.front();

    if (child) {
      UITapEvent localEvent = event;
      localEvent.x += scrollOffset_.x;
      localEvent.y += scrollOffset_.y;
      return child->processTap(localEvent);
    }
    return false;
  }

 private:
  Offset scrollOffset_{0, 0};
  Offset targetScrollOffset_{0, 0};
  ScrollViewParam params_;
  float scrollSpeed_ = 2.0f;
  float smoothness_ = 0.2f;
  mutable std::vector<ComponentPtr> cached_children_;
};

class ScrollView : public StatelessComponent {
 public:
  ScrollView(const ScrollViewParam& param = {}, const UIKey& key = {}) : params_(param), StatelessComponent(key) {}
  ScrollView(const ComponentPtr& child, const ScrollViewParam& param = {}, const UIKey& key = {})
      : params_(param), StatelessComponent(key) {
    addChild(child);
  }

  ComponentPtr build() override { return getChild(); }

  RenderObjectPtr createRenderObject() const noexcept override {
    return std::make_shared<ScrollViewRenderObject>(params_);
  }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<ScrollViewRenderObject>(ro)->updateParams(params_);
  }

  // void layout(UIConstraints constraints) override;
  // void draw(SkCanvas* canvas) override;
  // const std::vector<std::shared_ptr<UIComponent>>& children() const override;

 protected:
  // void onHoverMove(Offset& mousePosition) noexcept override;
  // void onHoverExit() noexcept override;
  // void setCursorPosCallback(Offset offset) noexcept override;
  //
  // bool processChildTaps(const UITapEvent& event) override {
  //   if (params_.child) {
  //     UITapEvent localEvent = event;
  //     localEvent.x += scrollOffset_.x;
  //     localEvent.y += scrollOffset_.y;
  //
  //     return params_.child->processTap(localEvent);
  //   }
  //
  //   return false;
  // }

 private:
  ScrollViewParam params_;

  // Offset scrollOffset_{0, 0};
  // Offset targetScrollOffset_{0, 0};
  // float scrollSpeed_ = 2.0f;
  // float smoothness_ = 0.2f;
  // mutable std::vector<std::shared_ptr<UIComponent>> cached_children_;
};

// inline void ScrollView::update() {}

// inline void ScrollView::onHoverExit() noexcept {
//   fmt::println("ScrollView::onHoverExit");
//   setIsHovered(false);
// }

// inline void ScrollView::onHoverMove(Offset& mousePosition) noexcept {
//   if (!this->shouldHandleHover()) return;
//
//   UIRect boundsUIRect{getGobalBounds()};
//   bool isInside = hitTest(mousePosition, boundsUIRect);
//
//   if (hitTest(mousePosition, boundsUIRect)) {
//     if (!isHovered()) onHoverEnter();
//     fmt::println("onHoverEnter");
//   } else {
//     if (isHovered()) onHoverExit();
//   }
// }

// inline void ScrollView::setCursorPosCallback(Offset offset) noexcept {
//   if (!isHovered() || !this->shouldHandleHover()) return;
//   fmt::println("setCursorPosCallback: {}", isHovered());
//
//   if (params_.direction == ScrollDirection::Vertical) {
//     float delta = offset.y * scrollSpeed_;
// #if defined(SK_BUILD_FOR_MAC)
//     delta = -delta;
// #endif
//     targetScrollOffset_.y += delta;
//
//   } else if (params_.direction == ScrollDirection::Horizontal) {
//     float delta = offset.x * scrollSpeed_;
// #if defined(SK_BUILD_FOR_MAC)
//     delta = -delta;
// #endif
//     targetScrollOffset_.x += delta;
//   }
// }
