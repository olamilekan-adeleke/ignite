#pragma once

#include <memory>
#include "ui_alignment.hpp"
#include "ui_component.hpp"

struct SizedParam {
  UISize size = {0, 0};
  UIAlignment align = UIAlignment::Center;
  std::shared_ptr<UIComponent> child = nullptr;
};

class Sized : public UIComponent {
 public:
  Sized(const SizedParam &param = {});

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

 private:
  UISize size_;
  UIAlignment align_;
  std::shared_ptr<UIComponent> child_;
};
