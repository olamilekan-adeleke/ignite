#pragma once

#include <modules/skparagraph/include/FontCollection.h>

#include <memory>
#include <sstream>
#include <utility>

#include "foundation/foundation.hpp"
#include "render/render_object.hpp"
#include "ui_element/state_base_element.hpp"

struct IconParam {
  IconData icon;
  float size = 20;
  Color color = Color::Black();
};

class IconRenderObject : public RenderObject {
 public:
  IconRenderObject(const IconParam &params = {}) : params_(params) {}

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas *canvas) noexcept override;

  void update(const IconParam &params) noexcept { params_ = std::move(params); }

 private:
  IconParam params_;
};

class Icon : public Component {
 public:
  Icon(const IconParam &param) : params_(param) {}

  UIElementPtr createElement() override { return std::make_shared<LeafUIElement>(shared_from_this()); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<IconRenderObject>(params_); };

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<IconRenderObject>(ro)->update(params_);
  }

  void debugFillProperties(std::ostringstream &os, int indent) const noexcept override {
    Component::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "size: " << params_.size << "\n";
  }

 private:
  IconParam params_;
};
