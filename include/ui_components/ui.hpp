#pragma once

#include <fmt/base.h>

#include <memory>
#include <string>

#include "basic/ui_component.hpp"
#include "component/component.hpp"
#include "debug_assert.hpp"
#include "elements/alignment_box.hpp"
#include "elements/checkbox_render.hpp"
#include "elements/fixed_box.hpp"
#include "elements/icon_renderer.hpp"
#include "elements/image.hpp"
#include "elements/opacity.hpp"
#include "elements/separator.hpp"
#include "elements/text_renderer.hpp"
#include "elements/ui_view.hpp"
#include "foundation/utils/key.hpp"
#include "interactive_components/button.hpp"
#include "interactive_components/checkbox.hpp"
#include "interactive_components/text_field_renderer.hpp"
#include "layout/h_flex_box.hpp"
#include "layout/layout_box.hpp"
#include "layout/v_flex_box.hpp"

namespace UI {

inline std::shared_ptr<TextRenderer> Text(const std::string &str, const TextStyle &style = TextStyle()) {
  return std::make_shared<TextRenderer>(str, style);
}

inline std::shared_ptr<View> UIView(const ViewParams &params = {}) { return std::make_shared<View>(params); }

// inline std::shared_ptr<UIComponent> FixedBoxView(const FixedBoxParam &params = {}) {
//   return std::make_shared<FixedBox>(params);
// }

inline std::shared_ptr<OpacityComponent> OpacityView(const OpacityParams &params = {}) {
  return std::make_shared<OpacityComponent>(params);
}

// inline std::shared_ptr<UIComponent> UIImageView(const ImageParams &params = {}) {
//   return std::make_shared<UIImage>(params);
// }

// inline std::shared_ptr<UIComponent> UISeparator(const SeparatorParams &params = {}) {
//   return std::make_shared<Separator>(params);
// }

inline std::shared_ptr<UIComponent> UICheckBox(const CheckBoxParams &params = {}) {
  return std::make_shared<CheckBox>(params);
}

inline std::shared_ptr<UIComponent> UIButton(const ButtonParams &params = {}) {
  return std::make_shared<Button>(params);
}

inline std::shared_ptr<UIComponent> UITextField(const UITextFieldParams &params = {}) {
  return std::make_shared<TextFieldRenderer>(params);
}

namespace Flex {
inline std::shared_ptr<UIComponent> column(const VFlexParam &params = {}) {
  return std::make_shared<LayoutBox>(VFlexBox::create(params));
}

inline std::shared_ptr<LayoutBox> row(const HFlexParam &params = {}) {
  return std::make_shared<LayoutBox>(HFlexBox::create(params));
}
}  // namespace Flex
}  // namespace UI

namespace IgniteUI {

enum class ChildMode { Single, Multiple };
class Tag {
  ComponentPtr component;
  ChildMode mode;

 public:
  Tag(ComponentPtr c, ChildMode m = ChildMode::Single) : component(c), mode(m) {}

  Tag &operator()(const std::initializer_list<ComponentPtr> &children) {
    ASSERT_CHILD_COUNT(Helper::to_string(component), mode, children.size());

    for (const auto &child : children) component->addChild(child);
    return *this;
  }

  operator ComponentPtr() { return component; }
};

// Text Related Components
namespace Paragraphs {
inline ComponentPtr text(const std::string &text, const TextStyle &params = {}, const UIKey &key = AUTO_KEY) {
  return std::make_shared<Text>(text, params, key);
}

inline ComponentPtr icon(const IconParam &param, const UIKey &key = AUTO_KEY) {
  return std::make_shared<Icon>(param, key);
}
}  // namespace Paragraphs

inline ComponentPtr center(const ComponentPtr child) { return std::make_shared<AlignmentBox>(child); }

inline Tag Box(const FixedBoxParam &param, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<FixedBox>(param, key), ChildMode::Single);
}

inline Tag Opacity(const OpacityParams &param, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<OpacityComponent>(param, key), ChildMode::Single);
}

inline ComponentPtr Image(const ImageParams &params, const UIKey &key = AUTO_KEY) {
  return std::make_shared<UIImage>(params, key);
}

inline ComponentPtr Separator(const SeparatorParams &params = {}, const UIKey key = AUTO_KEY) {
  return std::make_shared<SeparatorComponent>(params, key);
}

}  // namespace IgniteUI
