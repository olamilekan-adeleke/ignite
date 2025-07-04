#include "opacity.hpp"
#include <include/core/SkCanvas.h>
#include <include/core/SkRect.h>

Opacity::Opacity(const OpacityParams& param) : param_(param) {
  // if (param_.opacity < 0.0f) param_.opacity = 0.0f;
  // if (param_.opacity > 1.0f) param_.opacity = 1.0f;
}

void Opacity::layout(float parentWidth, float parentHeight) {
  if (param_.child) {
    // param_.child->setPosition(0, 0);
    param_.child->layout(parentWidth, parentHeight);
  }

  bounds_.width = param_.child->getBounds().width;
  bounds_.height = param_.child->getBounds().height;
}

void Opacity::draw(SkCanvas* canvas) {
  // TODO: Recheck this design decision later
  if (!param_.child) return;

  float opacity = param_.opacity;
  if (opacity < 0.0f) opacity = 0.0f;
  if (opacity > 1.0f) opacity = 1.0f;

  // Doc link for what opacity creation, turns out opacity is just setting the layer color aplha, lmao
  // https://skia-doc.commondatastorage.googleapis.com/doxygen/doxygen/html/classSkCanvas.html#a853514477dff62c15ced03f5141b2eb7

  // Compute alpha byte [0..255] from opacity float [0..1]
  U8CPU alphaValue = static_cast<U8CPU>(opacity * 255.0f);

  SkRect bounds = SkRect::MakeXYWH(bounds_.x, bounds_.y, bounds_.width, bounds_.height);
  canvas->saveLayerAlpha(&bounds, alphaValue);

  canvas->translate(bounds_.x, bounds_.y);
  param_.child->draw(canvas);
  canvas->restore();

  UIComponent::draw(canvas);
}
