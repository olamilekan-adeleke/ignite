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
#include "interactive_components/checkbox.hpp"
#include "interactive_components/text_field_renderer.hpp"
#include "layout/h_flex_box.hpp"
#include "layout/layout_box.hpp"
#include "layout/scroll_view.hpp"
#include "layout/v_flex_box.hpp"
#include "transformer/transformer.hpp"

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

inline Tag center(const UIKey &key = AUTO_KEY) { return Tag(std::make_shared<AlignmentBox>(key), ChildMode::Single); }

inline Tag Box(const FixedBoxParam &param, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<FixedBox>(param, key), ChildMode::Single);
}

inline Tag View(const ViewParams &param, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<UIView>(param, key), ChildMode::Single);
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

namespace Flex {
inline Tag column(const VFlexParam &params = {}, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<LayoutBox>(VFlexBox::create(params, key)), ChildMode::Multiple);
}

inline Tag row(const HFlexParam &params = {}, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<LayoutBox>(HFlexBox::create(params, key)), ChildMode::Multiple);
}

inline Tag scroll(const ScrollViewParam &params = {}, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<ScrollView>(params, key), ChildMode::Multiple);
}
}  // namespace Flex

inline ComponentPtr CheckBox(const CheckBoxParams &param, const UIKey &key = AUTO_KEY) {
  return std::make_shared<UICheckBox>(param, key);
}

inline ComponentPtr TextField(const UITextFieldParams &param, const UIKey &key = AUTO_KEY) {
  return std::make_shared<TextFieldComponent>(param, key);
}

inline Tag transform(const TransformParam &param, const UIKey &key = AUTO_KEY) {
  return Tag(std::make_shared<TransformComponent>(param, key), ChildMode::Single);
}

// namespace Flex
}  // namespace IgniteUI
