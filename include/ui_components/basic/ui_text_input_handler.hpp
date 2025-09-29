#pragma once

#include <functional>

using KeyCallback = std::function<void(int, int, int, int)>;
using CharCallback = std::function<void(unsigned int)>;

class UITextInputHandler {
 public:
  UITextInputHandler() {};

  virtual void handleCharEvent(char letter) noexcept {};

  void setFocus(bool focus) noexcept { hasFocus_ = focus; }
  bool hasFocus() const noexcept { return hasFocus_; }

 protected:
  bool hasFocus_ = false;
};
