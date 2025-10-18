#include "basic/ui_tap_handler.hpp"
#include "elements/ui_view.hpp"
#include "foundation/foundation.hpp"
#include "stateful_component.hpp"

struct ButtonParams {
  std::shared_ptr<UIComponent> child = nullptr;
  bool enable = true;
  float radius = 4.0f;
  UIEdgeInsets insets = {8.0f, 16.0f, 8.0f, 16.0f};
  Color fillColor = Color::Blue();
  Color inactiveBorderColor = Color::Gray().withAlpha(255 * 0.2f);
  TapListener onTap = nullptr;
};

class Button : public StatefulComponent {
 public:
  Button(const ButtonParams& param = {}) : param_(param) {}

  std::shared_ptr<UIComponent> body() override {
    const TapListener onTap = param_.onTap;
    ViewParams viewParams{
        .insets = param_.insets,
        .backgroundColor = param_.enable ? param_.fillColor : param_.inactiveBorderColor,
        .borderRadius = param_.radius,
        .child = param_.child,
    };
    viewParams.onTap = [this, onTap](const UITapEvent& e) {
      if (param_.enable && onTap != nullptr) onTap(e);
    };

    return std::make_shared<View>(viewParams);
  }

 private:
  ButtonParams param_{};
};
