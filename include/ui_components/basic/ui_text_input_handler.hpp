#pragma once

#include <functional>

#include "foundation/inputs/keyboard_key_event.hpp"

using KeyCallback = std::function<void(int, int, int, int)>;
using CharCallback = std::function<void(unsigned int)>;

class UITextInputHandler {
 public:
  UITextInputHandler() = default;

  virtual void handleCharEvent(std::string letter) noexcept {};

  virtual void handleKeyEvent(KeyEvent& key) noexcept {};

  void setFocus(bool focus) noexcept { hasFocus_ = focus; }
  bool hasFocus() const noexcept { return hasFocus_; }

 protected:
  bool hasFocus_ = false;
};
