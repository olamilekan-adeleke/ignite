#pragma once

#include <fmt/base.h>
#include <include/core/SkImage.h>
#include <include/core/SkSurface.h>
#include <cstdint>
#include <string>
#include "basic/ui_component.hpp"
#include "hash_helper.hpp"

struct ImageParams {
  std::string path;
  float width = 0.0f;
  float height = 0.0f;
  float opacity = 1.0f;

  uint64_t layoutHashCode() const noexcept {
    const auto key = fmt::format("{}-{}", width, height);
    return fnv1a(key);
  }

  uint64_t drawHashCode() const noexcept {
    auto const key = fmt::format("{}-{}", path, opacity);
    return fnv1a(key);
  }
};

class UIImage : public UIComponent {
 public:
  UIImage(const ImageParams& params = {});

  void layout(UIConstraints size) override;
  void draw(SkCanvas* canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  void markHasDirty(const UIMarkDirtyType& type, const UIMarkDirtyCaller& caller) noexcept override;

 protected:
  ImageParams params_;
  sk_sp<SkImage> imageData_;

  bool loadImage() noexcept;

  void debugFillProperties(std::ostringstream& os, int indent) const override;
};
