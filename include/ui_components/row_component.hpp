#pragma once

#include <memory>
#include <vector>

#include "ui_components/ui_component.hpp"

struct RowParams {
  float spacing = 0.0f;
  MainAxisAlignment mainAxisAlignment = MainAxisAlignment::START;
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::START;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class Row : public UIComponent {
 public:
  Row(const RowParams &param = {});

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  virtual bool processChildTaps(const UITapEvent &event) override {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      if ((*it)->processTap(event)) return true;
    }
    return false;
  }

 private:
  float spacing_;
  MainAxisAlignment mainAxisAlignment_;
  CrossAxisAlignment crossAxisAlignment_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};
