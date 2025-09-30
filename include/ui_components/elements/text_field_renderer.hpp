#pragma once

#include <fmt/base.h>
#include <sys/stat.h>
#include <cstdint>
#include <string>
#include <cstring>
#include <vector>

#include "color.hpp"
#include "keyboard_key_event.hpp"
#include "text_style.hpp"
#include "ui_edge_insets.hpp"

#include "basic/ui_component.hpp"
#include "elements/paragraph_builder.hpp"

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

    setTapListener([&](const UITapEvent& event) {});
  }

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

 protected:
  std::string text() const { return std::string(buffer_.begin(), buffer_.end()); }

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
  if (index > buffer_.size()) index = cursorIndex_;
  cursorIndex_ = index;
}

inline bool TextFieldRenderer::processChildTaps(const UITapEvent& event) {
  // First, let parent class handle child processing
  bool childHandled = false;
  // UIComponent::processChildTaps(event);

  // If a child handled it, don't process this component
  // if (childHandled) return true;

  // DEBUG log (keep for now)
  fmt::println("DEBUG: Tap at ({:.2f}, {:.2f}) | Bounds: x={:.2f} y={:.2f} w={:.2f} h={:.2f}",
               event.x,
               event.y,
               bounds_.x,
               bounds_.y,
               bounds_.width,
               bounds_.height);

  // NEW: Hit test against bounds
  if (bounds_.contains(event.x, event.y)) {
    // Success: Use bounds for cursor/etc.
    fmt::println("{}: handled tap at bounds ({:.2f}, {:.2f})", __PRETTY_FUNCTION__, event.x, event.y);
    setFocus(true);
    return true;
  }

  return false;

  // NEW: Transform to local coords (assumes event in parent/ancestor space, bounds in same)
  // float localX = event.x - bounds_.x;
  // float localY = event.y - bounds_.y;
  //
  // // Hit test against local [0, w] x [0, h]
  // if (localX >= 0.0f && localX <= bounds_.width && localY >= 0.0f && localY <= bounds_.height) {
  //   // Success: Use local for cursor/etc.
  //   setFocus(true);
  //   // Update cursorIndex_ based on localX (as in previous bonus)
  //   fmt::println("{}: handled tap at local ({:.2f}, {:.2f})", __PRETTY_FUNCTION__, localX, localY);
  //   return true;
  // }
  //
  // fmt::println("{}: tap missed", __PRETTY_FUNCTION__);
  // return false;
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
