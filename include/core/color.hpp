#pragma once

#include <include/core/SkColor.h>

#include <cstdint>
#include <random>

// Custom Color class to wrap SKColor (I Love doing things my way, hopefully i do not regret this)
struct Color {
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

  explicit Color(uint32_t hex);

  // Implicit conversion to SkColor
  operator SkColor() const;

  // Accessors
  uint8_t r() const;
  uint8_t g() const;
  uint8_t b() const;
  uint8_t a() const;

  // Return a copy with new alpha
  Color withAlpha(uint8_t newAlpha) const;

  // Named constants
  static Color Black();
  static Color White();
  static Color Red();
  static Color Green();
  static Color Blue();
  static Color Yellow();
  static Color Cyan();
  static Color Magenta();
  static Color Gray();
  static Color LightGray();
  static Color DarkGray();
  static Color Clear();

  static Color Random();

 private:
  SkColor value;
};
