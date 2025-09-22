#pragma once

#include <fmt/format.h>
#include "basic/ui_component.hpp"
#include "color.hpp"
#include "hash_helper.hpp"
#include "layout/flex_box.hpp"

struct SeparatorParams {
  Axis axis = Axis::VERTICAL;
  float thinkness = 1.0f;
  Color backgroundColor = Color::Gray();

  uint64_t layoutHashCode() const noexcept {
    const auto key = fmt::format("{}-{}", axisToString(axis), thinkness);
    return fnv1a(key);
  }

  uint64_t drawHashCode() const noexcept {
    const auto key = fmt::format("{}-{}-{}", axisToString(axis), thinkness, backgroundColor.toString());
    return fnv1a(key);
  }
};

class Separator : public UIComponent {
 public:
  Separator(const SeparatorParams& param = {}) : params_(param) {}

  void layout(UISize size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  void markHasDirty(const UIMarkDirtyType& type, const UIMarkDirtyCaller& caller) noexcept override;

 protected:
  void debugFillProperties(std::ostringstream& os, int indent) const override;

 private:
  SeparatorParams params_;
};

inline void Separator::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "axis: " << params_.axis << "\n";
}
