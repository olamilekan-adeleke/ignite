#pragma once

#include <memory>
#include <string>

#include "basic/ui_component.hpp"
#include "elements/checkbox_render.hpp"
#include "elements/fixed_box.hpp"
#include "elements/image.hpp"
#include "elements/opacity.hpp"
#include "elements/separator.hpp"
#include "elements/text_renderer.hpp"
#include "elements/ui_view.hpp"
#include "interactive_components/button.hpp"
#include "interactive_components/checkbox.hpp"
#include "interactive_components/text_field_renderer.hpp"
#include "layout/flex_box.hpp"
#include "layout/h_flex_box.hpp"
#include "layout/v_flex_box.hpp"

namespace UI {

inline std::shared_ptr<TextRenderer> Text(const std::string &str, const TextStyle &style = TextStyle()) { return std::make_shared<TextRenderer>(str, style); }

inline std::shared_ptr<FlexBox> UIFlexBox(const FlexParam &params = {}) { return std::make_shared<FlexBox>(params); }

inline std::shared_ptr<FlexBox> HFlexBox(const HFlexParam &params = {}) { return std::make_shared<FlexBox>(HFlexBox::create(params)); }

inline std::shared_ptr<FlexBox> VFlexBox(const VFlexParam &params = {}) { return std::make_shared<FlexBox>(VFlexBox::create(params)); }

inline std::shared_ptr<View> UIView(const ViewParams &params = {}) { return std::make_shared<View>(params); }

inline std::shared_ptr<UIComponent> FixedBoxView(const FixedBoxParam &params = {}) { return std::make_shared<FixedBox>(params); }

inline std::shared_ptr<Opacity> OpacityView(const OpacityParams &params = {}) { return std::make_shared<Opacity>(params); }

inline std::shared_ptr<UIComponent> UIImageView(const ImageParams &params = {}) { return std::make_shared<UIImage>(params); }

inline std::shared_ptr<UIComponent> UISeparator(const SeparatorParams &params = {}) { return std::make_shared<Separator>(params); }

inline std::shared_ptr<UIComponent> UICheckBox(const CheckBoxParams &params = {}) { return std::make_shared<CheckBox>(params); }

inline std::shared_ptr<UIComponent> UIButton(const ButtonParams &params = {}) { return std::make_shared<Button>(params); }

inline std::shared_ptr<UIComponent> UITextField(const UITextFieldParams &params = {}) { return std::make_shared<TextFieldRenderer>(params); }

// inline std::shared_ptr<UITextField> UITextFieldView(const UITextFieldParams &params = {}) {
//   return std::make_shared<UITextField>(params);
// }

// Fluent UI builder namespaces
// namespace Flex {
//   inline std::shared_ptr<FlexBox> column(const std::vector<std::shared_ptr<UIComponent>> &children) {
//     return UI::VFlexBox({.children = children});
//   }

//   inline std::shared_ptr<FlexBox> row(const std::vector<std::shared_ptr<UIComponent>> &children) {
//     return UI::HFlexBox({.children = children});
//   }
// }

// namespace Text {
//   inline std::shared_ptr<TextRenderer> create(const std::string &text, const TextStyle &style = TextStyle()) {
//     return UI::Text(text, style);
//   }
// }

// namespace Button {
//   inline std::shared_ptr<UIComponent> create(const std::string &label, const TapListener &onTap = nullptr) {
//     return UI::UIView({
//       .insets = {.top = 8, .left = 16, .bottom = 8, .right = 16},
//       .backgroundColor = Color::Blue(),
//       .borderRadius = 5,
//       .onTap = onTap,
//       .child = UI::Text(label, {.color = Color::White(), .fontSize = 16})
//     });
//   }
// }

}  // namespace UI
