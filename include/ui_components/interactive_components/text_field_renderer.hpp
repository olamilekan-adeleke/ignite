#pragma once

#include <fmt/base.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "component/component.hpp"
#include "elements/paragraph_builder.hpp"
#include "foundation/foundation.hpp"
#include "ui_element/state_base_element.hpp"

struct UITextFieldParams {
  UIEdgeInsets padding{8.0f, 12.0f, 8.0f, 12.0f};
  Color backgroundColor = Color::White();
  Color borderColor = Color::LightGray();
  float radius = 6.0f;
  std::string placeholder = "Enter text...";
  TextStyle placeholderStyle{.color = Color::Gray(), .fontSize = 14.0f};
  std::string value;
  TextStyle textStyle{.color = Color::Black(), .fontSize = 14.0f};
  UISizing size = UISizing{};
  bool multiline = false;
  ValueChangedListener<std::string> onChanged = nullptr;
};

class TextFieldRenderer : public RenderObject {
 public:
  TextFieldRenderer(const UITextFieldParams& params)
      : params_(params),
        textValueParagraph_(params_.value, params_.textStyle),
        placeholderParagraph_(params_.placeholder, params_.placeholderStyle) {
    if (!params_.value.empty()) {
      size_t position = 0;
      while (position < params_.value.size()) {
        std::string utf8Char = extractUtf8Char(params_.value, position);
        if (!utf8Char.empty()) buffer_.push_back(utf8Char);
      }
      cursorIndex_ = static_cast<uint32_t>(buffer_.size());
    }

    lastBlinkTime_ = std::chrono::steady_clock::now();
    cursorVisible_ = true;

    setTapListener([this](const UITapEvent& event) { onTextFieldTap(); });
  }

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;

  void updateParam(const UITextFieldParams& params) noexcept { params_ = std::move(params); }

  void onTextFieldTap() noexcept;

  void insertLetter(std::string letter) noexcept {
    auto insertIdx = buffer_.begin() + cursorIndex_;
    if (insertIdx >= buffer_.end()) {
      insertIdx = buffer_.end();
    }

    buffer_.insert(insertIdx, letter);
    setCursorIndex(cursorIndex_ + 1);

    std::string newText = text();
    textValueParagraph_.setText(newText);
    if (params_.onChanged) params_.onChanged(text());
  }

  void deleteLetter(uint32_t index) noexcept {
    if (buffer_.empty() || index >= buffer_.size()) return;

    auto deleteIdx = buffer_.begin() + index;
    buffer_.erase(deleteIdx);
    setCursorIndex(cursorIndex_ - 1);

    const std::string& newText = text();
    textValueParagraph_.setText(newText);
    if (params_.onChanged) params_.onChanged(text());
  }

  void handleCharEvent(std::string letter) noexcept override {
    if (!hasFocus()) return;
    size_t position = 0;
    while (position < letter.length()) {
      std::string utf8Char = extractUtf8Char(letter, position);
      if (!utf8Char.empty()) insertLetter(utf8Char);
    }
  }

  void handleKeyEvent(KeyEvent& key) noexcept override {
    if (!hasFocus()) return;
    if (key.key == KeyboardKey::BACKSPACE) {
      if (cursorIndex_ > 0) deleteLetter(cursorIndex_ - 1);
    } else if (key.key == KeyboardKey::LEFT) {
      if (cursorIndex_ > 0) setCursorIndex(cursorIndex_ - 1);
    } else if (key.key == KeyboardKey::RIGHT) {
      if (cursorIndex_ < buffer_.size()) setCursorIndex(cursorIndex_ + 1);
    } else if (key.key == KeyboardKey::ENTER) {
      if (params_.multiline) insertLetter("\n");
    } else if (key.key == KeyboardKey::TAB) {
      insertLetter("\t");
    }
  }

  void setCursorIndex(uint32_t index) noexcept {
    cursorIndex_ = std::min(index, static_cast<uint32_t>(buffer_.size()));
  }

 protected:
  std::string text() const {
    std::string value;
    for (const auto& letter : buffer_) value += letter;
    return value;
  }

 private:
  UITextFieldParams params_;
  ParagraphBuilder textValueParagraph_;
  ParagraphBuilder placeholderParagraph_;

  std::chrono::steady_clock::time_point lastBlinkTime_;
  static constexpr int CURSOR_BLINK_INTERVAL_MS = 350;
  bool cursorVisible_ = true;

  float lastTextFieldHeight_ = 0;
  uint32_t cursorIndex_ = 0;
  std::vector<std::string> buffer_{};
};

class TextFieldComponent : public Component {
 public:
  TextFieldComponent(const UITextFieldParams& params, const UIKey key = {}) : params_(params), Component(key) {}

  UIElementPtr createElement() override { return std::make_shared<LeafUIElement>(shared_from_this()); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<TextFieldRenderer>(params_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {}

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    Component::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
  }

 private:
  UITextFieldParams params_;
};

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
