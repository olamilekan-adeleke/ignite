#pragma once

#include <modules/skparagraph/include/FontCollection.h>

#include <sstream>

#include "basic/ui_component.hpp"
#include "color.hpp"
#include "icons/icon_types.hpp"
#include "ui_manager.hpp"

struct IconParam {
  IconData icon;
  float size = 20;
  Color color = Color::Black();
};

class IconRenderer : public UIComponent {
 public:
  IconRenderer(const IconParam &param) : params_(param) {}

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

 private:
  IconParam params_;
};

inline void IconRenderer::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "size: " << params_.size << "\n";
}
