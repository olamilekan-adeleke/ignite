#include "elements/image.hpp"
#include <fmt/base.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkPaint.h>
#include <include/core/SkStream.h>
#include <algorithm>
#include <filesystem>
#include <string>
#include <sstream>
#include "size.hpp"

UIImage::UIImage(const ImageParams& param) : params_(param) {
  auto cwd = std::filesystem::current_path().string();

  std::string s = "/";
  std::string subpath = params_.path;
  if (!std::equal(cwd.rbegin(), cwd.rend(), s.rbegin())) {
    cwd += "/";
  }
  if (!std::equal(cwd.rbegin(), cwd.rend(), s.rbegin())) {
    subpath += "/";
  }

  std::string fullPath = cwd + params_.path.c_str();
  auto stream = SkStream::MakeFromFile(fullPath.c_str());
  if (!stream) {
    fmt::println("ImageView: Failed to open {}", fullPath);
    return;
  }

  sk_sp<SkData> data = SkData::MakeFromStream(stream.get(), stream->getLength());
  if (!data) {
    fmt::println("ImageView: Failed to read data {}", fullPath);
    return;
  }
  fmt::println("ImageView: Read {} bytes", data->size());

  imageData_ = SkImages::DeferredFromEncodedData(data);
}

UISize UIImage::getIntrinsicSize(UIConstraints constraints) noexcept {
  return UISize{.width = params_.width, .height = params_.height};
}

void UIImage::layout(UISize size) {
  if (!imageData_) return;
  bounds_.width = params_.width > 0 ? params_.width : imageData_->width();
  bounds_.height = params_.height > 0 ? params_.height : imageData_->height();
}

void UIImage::draw(SkCanvas* canvas) {
  if (!imageData_) return;

  SkPaint paint;
  paint.setAlphaf(params_.opacity);
  // canvas->translate(bounds_.x, bounds_.y);

  SkRect dest = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  canvas->drawImageRect(imageData_, dest, SkSamplingOptions(), &paint);
  UIComponent::draw(canvas);
}

void UIImage::debugFillProperties(std::ostringstream& os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << "image height: " << params_.height << "\n";
  os << "image width: " << params_.width << "\n";
  os << "opacity: " << params_.opacity << "\n";
  os << "path: " << params_.path << "\n";
};
