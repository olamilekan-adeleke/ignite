#pragma once

#include <memory>
#include <string>

#include "column_component.hpp"
#include "text_component.hpp"

namespace UI {

inline std::shared_ptr<TextComponent> Text(const std::string& str, float fontSize = 16.0f, Color color = Color::Black(),
                                           FontWeight weight = FontWeight::Normal, bool underline = false,
                                           bool italic = false) {
  return std::make_shared<TextComponent>(str, color, fontSize, weight, underline, italic);
}

inline std::shared_ptr<Column> ColumnView(std::initializer_list<std::shared_ptr<UIComponent>> children) {
  auto col = std::shared_ptr<Column>(new Column());
  for (auto& c : children) {
    col->addChild(c);
  }
  return col;
}
}  // namespace UI
