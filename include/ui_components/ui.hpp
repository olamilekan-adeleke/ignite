#pragma once

#include <memory>
#include <string>

#include "column_component.hpp"
#include "layout/h_flex_box.hpp"
#include "layout/v_flex_box.hpp"
#include "ui_component.hpp"
#include "ui_sized.hpp"
#include "row_component.hpp"
#include "text_component.hpp"
#include "ui_text_field.hpp"
#include "ui_view.hpp"
#include "opacity.hpp"
#include "image.hpp"
#include "layout/flex_box.hpp"

namespace UI {

inline std::shared_ptr<TextRenderer> Text(const std::string &str, const TextStyle &style = TextStyle()) {
  return std::make_shared<TextRenderer>(str, style);
}

inline std::shared_ptr<Column> ColumnView(const ColumnParams &props = {}) { return std::make_shared<Column>(props); }

inline std::shared_ptr<Row> RowView(const RowParams &props = {}) { return std::make_shared<Row>(props); }

inline std::shared_ptr<View> UIView(const ViewParams &params = {}) { return std::make_shared<View>(params); }

inline std::shared_ptr<UIComponent> SizedView(const SizedParam &params = {}) { return std::make_shared<Sized>(params); }

inline std::shared_ptr<Opacity> OpacityView(const OpacityParams &params = {}) {
  return std::make_shared<Opacity>(params);
}

inline std::shared_ptr<UIComponent> UIImageView(const ImageParams &params = {}) {
  return std::make_shared<UIImage>(params);
}

inline std::shared_ptr<UITextField> UITextFieldView(const UITextFieldParams &params = {}) {
  return std::make_shared<UITextField>(params);
}

inline std::shared_ptr<FlexBox> UIFlexBox(const FlexParam &params = {}) { return std::make_shared<FlexBox>(params); }

inline std::shared_ptr<FlexBox> HFlexBox(const HFlexParam &params = {}) {
  return std::make_shared<FlexBox>(HFlexBox::create(params));
}

inline std::shared_ptr<FlexBox> VFlexBox(const VFlexParam &params = {}) {
  return std::make_shared<FlexBox>(VFlexBox::create(params));
}
}  // namespace UI
