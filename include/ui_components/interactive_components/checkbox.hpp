#include "stateful_component.hpp"
#include "elements/checkbox_render.hpp"

class CheckBox : public StatefulComponent {
 public:
  CheckBox(const CheckBoxParams param = {}) : param_(param) {}

  std::shared_ptr<UIComponent> body() override {
    auto originalOnTap = param_.onTap;

    const CheckBoxParams current{
        .enable = param_.enable,
        .size = param_.size,
        .radius = param_.radius,
        .inactiveBorderColor = param_.inactiveBorderColor,
        .checkColor = param_.checkColor,
        .onTap =
            [this, originalOnTap](const UITapEvent& e) {
              fmt::println("CheckBox tapped!");
              param_.enable = !param_.enable;
              markDirty();

              if (originalOnTap != nullptr) originalOnTap(e);
            },
    };

    return std::make_shared<CheckBoxRender>(current);
  }

 private:
  CheckBoxParams param_{};
};
