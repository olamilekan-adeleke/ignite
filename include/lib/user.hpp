#pragma once

#include "skia/SkiaRenderer.hpp"

class UserPage {
 public:
  UserPage(SkiaRenderer* renderer);
  ~UserPage();

  void draw();

 private:
  SkiaRenderer* skiaRenderer;
  SkPaint textPaint;
  SkFont font;

  void setupPaint();
};
