#include "elements/image.hpp"

#include <fmt/base.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkPaint.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <sstream>
#include <string>

#include "size.hpp"
#include "surface_cache_helper.hpp"
#include "ui_manager.hpp"

struct ImageCacheManager {
  std::unordered_map<std::string, sk_sp<SkImage>> imageCache_;

  static ImageCacheManager& getInstance() {
    static ImageCacheManager instance;
    return instance;
  }

 private:
  ImageCacheManager() = default;

  ImageCacheManager(const ImageCacheManager&) = delete;
  ImageCacheManager& operator=(const ImageCacheManager&) = delete;
};

UIImage::UIImage(const ImageParams& param) : params_(param) { loadImage(); }

void UIImage::markHasDirty(const UIMarkDirtyType& type, const UIMarkDirtyCaller& caller) noexcept {
  const auto currentLayoutHash{params_.layoutHashCode()};
  const auto currentDrawHash{params_.drawHashCode()};

  if (type == UIMarkDirtyType::DRAW) {
    UICacheManager::instance().removeCachedSurface(currentDrawHash);
    return;
  }

  UICacheManager::instance().removeCachedSurface(currentDrawHash);
  UICacheManager::instance().removeLayoutCached(currentLayoutHash);
};

bool UIImage::loadImage() noexcept {
  // Check if image is already cached
  const auto pathHash = toHex(fnv1a(params_.path));
  auto cacheIt = ImageCacheManager::getInstance().imageCache_.find(pathHash);
  if (cacheIt != ImageCacheManager::getInstance().imageCache_.end()) {
    imageData_ = cacheIt->second;
    return true;
  }

  std::string fullPath = std::filesystem::current_path().string() + "/" + params_.path;
  fullPath.erase(std::unique(fullPath.begin(), fullPath.end(), [](char a, char b) { return a == '/' && b == '/'; }),
                 fullPath.end());

  const auto stream = SkStream::MakeFromFile(fullPath.c_str());
  if (!stream) {
    fmt::println("ImageView: Failed to open {}", fullPath);
    return false;
  }

  const sk_sp<SkData> data = SkData::MakeFromStream(stream.get(), stream->getLength());
  if (!data) {
    fmt::println("ImageView: Failed to read data {}", fullPath);
    return false;
  }

  imageData_ = SkImages::DeferredFromEncodedData(data);
  fmt::println("Loaded image: {} | {}", pathHash, fullPath);
  if (imageData_) ImageCacheManager::getInstance().imageCache_.insert({pathHash, imageData_});
  return imageData_ != nullptr;
}

UISize UIImage::getIntrinsicSize(UIConstraints constraints) noexcept {
  if (!imageData_) {
    return UISize{.width = params_.width, .height = params_.height};
  }

  float width = params_.width > 0 ? params_.width : imageData_->width();
  float height = params_.height > 0 ? params_.height : imageData_->height();
  return UISize{.width = width, .height = height};
}

void UIImage::layout(UIConstraints size) {
  const uint64_t currentLayoutHash{params_.layoutHashCode()};
  const auto cacheIt = UICacheManager::instance().getLayoutCached(currentLayoutHash);
  if (cacheIt.has_value()) {
    bounds_ = *cacheIt;
    return;
  }

  if (!imageData_) return;
  // fmt::println("layouting image: {} [currentLayoutHash: {}]", params_.path, currentLayoutHash);
  bounds_.width = params_.width > 0 ? params_.width : imageData_->width();
  bounds_.height = params_.height > 0 ? params_.height : imageData_->height();

  UICacheManager::instance().setLayoutCached(currentLayoutHash, bounds_);
  markHasDirty(UIMarkDirtyType::DRAW, UIMarkDirtyCaller::NONE);
}

// void UIImage::draw(SkCanvas* canvas) {
//   const uint64_t currentDrawHash{params_.drawHashCode()};
//   sk_sp<SkSurface> cachedSurface = UICacheManager::instance().getCachedSurface(currentDrawHash);
//
//   if (cachedSurface) {
//     canvas->drawImage(cachedSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
//     UIComponent::draw(canvas);
//     return;
//   }
//
//   if (!imageData_) return;
//   SkImageInfo info = SkImageInfo::Make(bounds_.width, bounds_.height, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
//   sk_sp<SkSurface> newSurface = SkSurfaces::Raster(info);
//   if (!newSurface) return;
//
//   SkCanvas* surfaceCanvas = newSurface->getCanvas();
//   SkPaint paint;
//   paint.setAlphaf(params_.opacity);
//
//   SkRect dest = SkRect::MakeXYWH(0, 0, bounds_.width, bounds_.height);
//   surfaceCanvas->drawImageRect(imageData_, dest, SkSamplingOptions(), &paint);
//
//   UICacheManager::instance().setCachedSurface(currentDrawHash, newSurface);
//   canvas->drawImage(newSurface->makeImageSnapshot(), bounds_.x, bounds_.y);
//   UIComponent::draw(canvas);
// }
void UIImage::draw(SkCanvas* canvas) {
  if (!imageData_) return;

  const uint64_t drawHash{params_.drawHashCode()};
  SkRect bounds = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);

  SurfaceCacheHelper::drawCached(canvas, drawHash, bounds, [&](SkCanvas* surfaceCanvas) {
    SkPaint paint;
    paint.setAlphaf(params_.opacity);
    SkRect dest = SkRect::MakeXYWH(0, 0, bounds_.width, bounds_.height);
    surfaceCanvas->drawImageRect(imageData_, dest, SkSamplingOptions(), &paint);
  });

  UIComponent::draw(canvas);
}

void UIImage::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "height: " << params_.height << "\n";
  os << pad << "width: " << params_.width << "\n";
  os << pad << "opacity: " << params_.opacity << "\n";
  os << pad << fmt::format("path: \"{}\"", params_.path) << "\n";
};
