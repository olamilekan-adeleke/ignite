#pragma once

#include <memory>
#include <string>

#include "basic/ui_component.hpp"

#include "elements/image.hpp"
#include "elements/text_renderer.hpp"
#include "elements/ui_view.hpp"
#include "elements/opacity.hpp"
#include "elements/separator.hpp"
#include "elements/checkbox_render.hpp"

#include "layout/flex_box.hpp"
#include "layout/h_flex_box.hpp"
#include "layout/v_flex_box.hpp"

#include "interactive_components/checkbox.hpp"
#include "interactive_components/button.hpp"

#include "ui_sized.hpp"
#include "ui_text_field.hpp"

namespace UI {

inline std::shared_ptr<TextRenderer> Text(const std::string &str, const TextStyle &style = TextStyle()) {
  return std::make_shared<TextRenderer>(str, style);
}

inline std::shared_ptr<FlexBox> UIFlexBox(const FlexParam &params = {}) { return std::make_shared<FlexBox>(params); }

inline std::shared_ptr<FlexBox> HFlexBox(const HFlexParam &params = {}) {
  return std::make_shared<FlexBox>(HFlexBox::create(params));
}

inline std::shared_ptr<FlexBox> VFlexBox(const VFlexParam &params = {}) {
  return std::make_shared<FlexBox>(VFlexBox::create(params));
}

inline std::shared_ptr<View> UIView(const ViewParams &params = {}) { return std::make_shared<View>(params); }

inline std::shared_ptr<UIComponent> SizedView(const SizedParam &params = {}) { return std::make_shared<Sized>(params); }

inline std::shared_ptr<Opacity> OpacityView(const OpacityParams &params = {}) {
  return std::make_shared<Opacity>(params);
}

inline std::shared_ptr<UIComponent> UIImageView(const ImageParams &params = {}) {
  return std::make_shared<UIImage>(params);
}

inline std::shared_ptr<UIComponent> UISeparator(const SeparatorParams &params = {}) {
  return std::make_shared<Separator>(params);
}

inline std::shared_ptr<UIComponent> UICheckBox(const CheckBoxParams &params = {}) {
  return std::make_shared<CheckBox>(params);
}

inline std::shared_ptr<UIComponent> UIButton(const ButtonParams &params = {}) {
  return std::make_shared<Button>(params);
}

inline std::shared_ptr<UITextField> UITextFieldView(const UITextFieldParams &params = {}) {
  return std::make_shared<UITextField>(params);
}

}  // namespace UI
