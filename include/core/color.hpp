#pragma once

#include <cstdint>
#include <string>

using SkColor = uint32_t;

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
  Color withAlpha(uint8_t alpha) const;

  // Basic colors
  static Color Black();
  static Color White();
  static Color Clear();

  // Primary colors
  static Color Red();
  static Color Green();
  static Color Blue();

  // Secondary colors
  static Color Yellow();
  static Color Cyan();
  static Color Magenta();

  // Grayscale
  static Color Gray();
  static Color LightGray();
  static Color DarkGray();

  // Additional common colors
  static Color Orange();
  static Color Purple();
  static Color Pink();
  static Color Brown();
  static Color Lime();
  static Color Teal();
  static Color Navy();
  static Color Maroon();
  static Color Olive();
  static Color Indigo();
  static Color Violet();
  static Color Gold();
  static Color Silver();

  // Modern UI colors
  static Color SlateGray();
  static Color DimGray();
  static Color Crimson();
  static Color Coral();
  static Color Tomato();
  static Color SkyBlue();
  static Color SteelBlue();
  static Color Turquoise();
  static Color Aquamarine();
  static Color Salmon();
  static Color Khaki();
  static Color Plum();
  static Color Orchid();

  // Random color generator
  static Color Random();

  std::string toString() const;

 private:
  SkColor value;
};
