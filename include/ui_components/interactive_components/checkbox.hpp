#include "elements/checkbox_render.hpp"
#include "stateful_component.hpp"

class CheckBox : public StatefulComponent {
 public:
  CheckBox(const CheckBoxParams& param = {}) : param_(param) {}

  std::shared_ptr<UIComponent> body() override {
    auto originalOnTap = param_.onTap;

    CheckBoxParams current{param_};
    current.onTap = [this, originalOnTap](const UITapEvent& e) {
      fmt::println("CheckBox tapped!");
      param_.checked = !param_.checked;
      markDirty();

      if (originalOnTap != nullptr) originalOnTap(e);
    };

    return std::make_shared<CheckBoxRender>(current);
  }

 private:
  CheckBoxParams param_{};
};
