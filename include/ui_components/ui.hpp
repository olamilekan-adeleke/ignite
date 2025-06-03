#pragma once

#include <memory>
#include <string>

#include "text_component.hpp"

namespace UI {

inline std::shared_ptr<TextComponent> Text(const std::string& str, float fontSize = 16.0f, Color color = Color::Black(),
                                           FontWeight weight = FontWeight::Normal, bool underline = false,
                                           bool italic = false) {
  return std::make_shared<TextComponent>(str, color, fontSize, weight, underline, italic);
}

}  // namespace UI
