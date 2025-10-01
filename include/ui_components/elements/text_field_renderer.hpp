#pragma once

#include <fmt/base.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "color.hpp"
#include "keyboard_key_event.hpp"
#include "text_style.hpp"
#include "ui_edge_insets.hpp"

#include "basic/ui_component.hpp"
#include "elements/paragraph_builder.hpp"
#include "ui_manager.hpp"

struct UITextFieldParams {
  UIEdgeInsets padding{8.0f, 12.0f, 8.0f, 12.0f};
  Color backgroundColor = Color::White();
  Color borderColor = Color::LightGray();
  float radius = 6.0f;
  std::string placeholder = "Enter text...";
  TextStyle placeholderStyle{.color = Color::Gray(), .fontSize = 14.0f};
  std::string value;
  TextStyle textStyle{.color = Color::Black(), .fontSize = 14.0f};
  float width = 120.0f;
  float minHeight = 20.0f;
};

class TextFieldRenderer : public UIComponent {
 public:
  TextFieldRenderer(const UITextFieldParams& params)
      : params_(params),
        textValueParagraph_(params_.value, params_.textStyle),
        placeholderParagraph_(params_.placeholder, params_.placeholderStyle) {
    lastBlinkTime_ = std::chrono::steady_clock::now();
    cursorVisible_ = true;

    setTapListener([this](const UITapEvent& event) { onTextFieldTap(); });
  }

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

 protected:
  std::string_view text() const { return std::string_view(buffer_.data(), buffer_.size()); }

  void onTextFieldTap() noexcept;

  void insertLetter(char letter) noexcept;

  void deleteLetter(uint32_t index) noexcept;

  void handleCharEvent(char letter) noexcept override;

  void handleKeyEvent(KeyEvent& key) noexcept override;

  void setCursorIndex(uint32_t index) noexcept;

  void debugFillProperties(std::ostringstream& os, int indent) const override;

  bool processChildTaps(const UITapEvent& event) override;

 private:
  UITextFieldParams params_;
  ParagraphBuilder textValueParagraph_;
  ParagraphBuilder placeholderParagraph_;

  std::chrono::steady_clock::time_point lastBlinkTime_;
  static constexpr int CURSOR_BLINK_INTERVAL_MS = 350;
  bool cursorVisible_ = true;

  float lastTextFieldHeight_ = 0;
  uint32_t cursorIndex_ = 0;
  std::vector<char> buffer_{};
};

inline void TextFieldRenderer::onTextFieldTap() noexcept { UIManager::instance().requestFocus(*this); }

inline void TextFieldRenderer::insertLetter(char letter) noexcept {
  auto insertIdx = buffer_.begin() + cursorIndex_;
  if (insertIdx >= buffer_.end()) {
    insertIdx = buffer_.end();
  }

  buffer_.insert(insertIdx, letter);
  setCursorIndex(cursorIndex_ + 1);

  const std::string& newText = std::string(buffer_.begin(), buffer_.end());
  textValueParagraph_.setText(newText);
  fmt::println("DEBUG: New text: {}", std::string(buffer_.begin(), buffer_.end()));
}

inline void TextFieldRenderer::deleteLetter(uint32_t index) noexcept {
  if (buffer_.empty() || index >= buffer_.size()) return;

  auto deleteIdx = buffer_.begin() + index;
  buffer_.erase(deleteIdx);
  setCursorIndex(cursorIndex_ - 1);

  const std::string& newText = std::string(buffer_.begin(), buffer_.end());
  textValueParagraph_.setText(newText);
  fmt::println("DEBUG: New text: {}", std::string(buffer_.begin(), buffer_.end()));
}

inline void TextFieldRenderer::handleCharEvent(char letter) noexcept {
  if (!hasFocus()) return;
  insertLetter(letter);
}

inline void TextFieldRenderer::handleKeyEvent(KeyEvent& key) noexcept {
  if (!hasFocus()) return;
  if (key.key == KeyboardKey::BACKSPACE) {
    deleteLetter(cursorIndex_ - 1);
  } else if (key.key == KeyboardKey::LEFT) {
    setCursorIndex(cursorIndex_ - 1);
  } else if (key.key == KeyboardKey::RIGHT) {
    setCursorIndex(cursorIndex_ + 1);
  } else if (key.key == KeyboardKey::ENTER) {
    insertLetter('\n');
  } else if (key.key == KeyboardKey::TAB) {
    insertLetter('\t');
  }
}

inline void TextFieldRenderer::setCursorIndex(uint32_t index) noexcept {
  cursorIndex_ = std::min(index, static_cast<uint32_t>(buffer_.size()));
}

inline bool TextFieldRenderer::processChildTaps(const UITapEvent& event) {
  auto result = onTap(event, bounds_);
  setFocus(result);
  return result;
}

inline void TextFieldRenderer::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
}

// Rough cursor positioning: find char index closest to localEvent.x
// float cursorX = params_.padding.left;
// cursorIndex_ = 0;
// for (char c : text()) {
//   float charWidth = textValueParagraph_.getGlyphWidth(cursorIndex_);  // Assume method exists
//   if (localEvent.x < cursorX + charWidth / 2) break;
//   cursorX += charWidth;
//   ++cursorIndex_;
// }
// // Blink timer reset
// lastBlinkTime_ = std::chrono::steady_clock::now();
// cursorVisible_ = true;
