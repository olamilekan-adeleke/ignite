#pragma once

#include <memory>
#include <string>

#include "column_component.hpp"
#include "row_component.hpp"
#include "text_component.hpp"
#include "ui_view.hpp"

namespace UI {

inline std::shared_ptr<TextComponent> Text(const std::string &str, const TextStyle &style = TextStyle()) {
  return std::make_shared<TextComponent>(str, style);
}

inline std::shared_ptr<Column> ColumnView(const ColumnParams &props = {}) {
  return std::make_shared<Column>(props);
}

inline std::shared_ptr<Row> RowView(const RowParams &props = {}) {
  return std::make_shared<Row>(props);
}

inline std::shared_ptr<View> UIView(const ViewParams &params = {}) {
  return std::make_shared<View>(params);
}
} // namespace UI
