#pragma once

#include <fmt/base.h>
#include <include/core/SkImage.h>
#include <include/core/SkSurface.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "component/component.hpp"
#include "component/state_base_component.hpp"
#include "foundation/foundation.hpp"
#include "foundation/utils/key.hpp"
#include "render/render_object.hpp"
#include "ui_element/state_base_element.hpp"

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

class ImageRenderObject : public RenderObject {
 public:
  ImageRenderObject(const ImageParams& param) : params_(param) { loadImage(); }

  void performLayout(UIConstraints constraints) noexcept override;

  void paint(SkCanvas* canvas) noexcept override;

  void update(const ImageParams& param) { params_ = std::move(param); }

 private:
  ImageParams params_;
  sk_sp<SkImage> imageData_;

  bool loadImage() noexcept;
};

class UIImage : public Component {
 public:
  UIImage(const ImageParams& params = {}, const UIKey& key = {}) : params_(params), Component(key) {}

  UIElementPtr createElement() noexcept override { return std::make_shared<LeafUIElement>(shared_from_this()); }

  RenderObjectPtr createRenderObject() const noexcept override { return std::make_shared<ImageRenderObject>(params_); }

  void updateRenderObject(RenderObjectPtr ro) noexcept override {
    std::dynamic_pointer_cast<ImageRenderObject>(ro)->update(params_);
  }

 protected:
  ImageParams params_;

  void debugFillProperties(std::ostringstream& os, int indent) const noexcept override {
    Component::debugFillProperties(os, indent);
    std::string pad(indent, ' ');
    os << pad << "height: " << params_.height << "\n";
    os << pad << "width: " << params_.width << "\n";
    os << pad << "opacity: " << params_.opacity << "\n";
    os << pad << fmt::format("path: \"{}\"", params_.path) << "\n";
  }
};
