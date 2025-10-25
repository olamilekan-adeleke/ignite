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
#include <string>

#include "foundation/foundation.hpp"
#include "foundation/geometry/rect.hpp"
#include "render/render_object.hpp"
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

bool ImageRenderObject::loadImage() noexcept {
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

void ImageRenderObject::performLayout(UIConstraints size) noexcept {
  if (!imageData_) {
    setSize(std::clamp(0.0f, size.minWidth, size.maxWidth), std::clamp(0.0f, size.minHeight, size.maxHeight));
    return;
  }

  float imgWidth = imageData_->width();
  float imgHeight = imageData_->height();
  float width = params_.width > 0 ? params_.width : imgWidth;
  float height = params_.height > 0 ? params_.height : imgHeight;

  // Option: Maintain aspect ratio if only one dimension is specified
  if (params_.width > 0 && params_.height <= 0) {
    height = (params_.width / imgWidth) * imgHeight;
  } else if (params_.height > 0 && params_.width <= 0) {
    width = (params_.height / imgHeight) * imgWidth;
  }

  width = std::clamp(width, size.minWidth, size.maxWidth);
  height = std::clamp(height, size.minHeight, size.maxHeight);
  setSize(width, height);
}

void ImageRenderObject::paint(SkCanvas* canvas) noexcept {
  if (!imageData_) return;

  const uint64_t drawHash{params_.drawHashCode()};
  const UIRect& bounds_ = getBounds();
  SkRect bounds = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);

  SurfaceCacheHelper::drawCached(canvas, drawHash, bounds, [&](SkCanvas* surfaceCanvas) {
    SkPaint paint;
    paint.setAlphaf(params_.opacity);
    SkRect dest = SkRect::MakeXYWH(0, 0, bounds_.width, bounds_.height);
    surfaceCanvas->drawImageRect(imageData_, dest, SkSamplingOptions(), &paint);
  });

  RenderObject::paint(canvas);
}
