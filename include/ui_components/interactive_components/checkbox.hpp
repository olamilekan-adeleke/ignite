#include "component/state_base_component.hpp"
#include "elements/checkbox_render.hpp"

struct UICheckBoxState : public StateBase {
 public:
  UICheckBoxState(const CheckBoxParams &param, const UIKey &key) : params_(param), key_(key) {}

  void initState() override {
    auto originalOnTap = params_.onTap;
    params_.onTap = [this, originalOnTap](UITapEvent event) {
      updateState([this, originalOnTap, event]() mutable {
        params_.checked = !params_.checked;
        if (originalOnTap) originalOnTap(event);
      });
    };
  }
  void dispose() override {}

  ComponentPtr build() override { return std::make_shared<UICheckBoxComponent>(params_, key_); }

 private:
  CheckBoxParams params_;
  UIKey key_;
};

class UICheckBox : public StatefullyComponent {
 public:
  UICheckBox(const CheckBoxParams &param = {}, const UIKey &key = {})
      : params_(param), StatefullyComponent(key), key_(key) {}

  std::shared_ptr<StateBase> createState() override { return std::make_shared<UICheckBoxState>(params_, key_); }

 private:
  CheckBoxParams params_;
  UIKey key_;
};
