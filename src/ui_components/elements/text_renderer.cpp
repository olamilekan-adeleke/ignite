#include "elements/text_renderer.hpp"

#include <fmt/base.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPoint.h>

#include "foundation/foundation.hpp"
#include "ui_element/state_base_element.hpp"

// Text
UIElementPtr Text::createElement() { return std::make_shared<LeafUIElement>(shared_from_this()); }

RenderObjectPtr Text::createRenderObject() const noexcept { return std::make_shared<RenderText>(text_, style_); }

void Text::updateRenderObject(RenderObjectPtr ro) noexcept {
  std::dynamic_pointer_cast<RenderText>(ro)->setText(text_);
  std::dynamic_pointer_cast<RenderText>(ro)->setStyle(style_);
}
