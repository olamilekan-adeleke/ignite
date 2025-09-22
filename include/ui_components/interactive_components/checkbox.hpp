#include "stateful_component.hpp"
#include "elements/checkbox_render.hpp"

class CheckBox : public StatefulComponent {
 public:
  CheckBox(const CheckBoxParams& param = {}) : param_(param) {}

  std::shared_ptr<UIComponent> body() override {
    auto originalOnTap = param_.onTap;

    CheckBoxParams current{param_};
    current.onTap = [this, originalOnTap](const UITapEvent& e) {
      fmt::println("CheckBox tapped!");
      param_.enable = !param_.enable;
      markDirty();

      if (originalOnTap != nullptr) originalOnTap(e);
    };

    return std::make_shared<CheckBoxRender>(current);
  }

 private:
  CheckBoxParams param_{};
};
