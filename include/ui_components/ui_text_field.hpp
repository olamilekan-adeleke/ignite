#pragma once

#include <string>
#include <cstring>  // Added for strncpy
#include <imgui.h>
#include "ui_component.hpp"

struct UITextFieldParams {
  std::string placeholder;
  std::string initialText;
  float width = 150.0f;
  float height = 30.0f;
};

class UITextField : public UIComponent {
 public:
  UITextField(const UITextFieldParams& params) : UIComponent(), params_(params) {
    // Safer initialization with bounds checking
    size_t copyLen = std::min(params_.initialText.length(), sizeof(buffer_) - 1);
    std::memcpy(buffer_, params_.initialText.c_str(), copyLen);
    buffer_[copyLen] = '\0';
  }

  void layout(float parentWidth, float parentHeight) override {
    bounds_.width = params_.width;
    bounds_.height = params_.height;
  }

  void draw(SkCanvas* canvas) override {
    ImGui::SetNextItemWidth(bounds_.width);

    // Use placeholder text as label, or generate unique ID
    std::string label = params_.placeholder.empty() ? "##textfield" : params_.placeholder;

    if (ImGui::InputText(label.c_str(), buffer_, IM_ARRAYSIZE(buffer_))) {
      // Optional: handle text change events here
    }

    // Call parent draw method
    UIComponent::draw(canvas);
  }

  std::string text() const { return std::string(buffer_); }

  // Optional: Method to programmatically set text
  void setText(const std::string& newText) {
    size_t copyLen = std::min(newText.length(), sizeof(buffer_) - 1);
    std::memcpy(buffer_, newText.c_str(), copyLen);
    buffer_[copyLen] = '\0';
  }

 private:
  UITextFieldParams params_;
  char buffer_[256];
};
