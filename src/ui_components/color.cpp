#include "ui_components/color.hpp"

#include <include/core/SkColor.h>

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : value(SkColorSetARGB(a, r, g, b)) {}

Color::Color(uint32_t hex) {
  uint8_t a = (hex >> 24) & 0xFF;
  uint8_t r = (hex >> 16) & 0xFF;
  uint8_t g = (hex >> 8) & 0xFF;
  uint8_t b = hex & 0xFF;
  value = SkColorSetARGB(a, r, g, b);
}

Color::operator SkColor() const { return value; }

uint8_t Color::r() const { return SkColorGetR(value); }
uint8_t Color::g() const { return SkColorGetG(value); }
uint8_t Color::b() const { return SkColorGetB(value); }
uint8_t Color::a() const { return SkColorGetA(value); }

Color Color::withAlpha(uint8_t newAlpha) const { return Color(r(), g(), b(), newAlpha); }

// Named constants for easy color, will expand this list later on ot as i need them
Color Color::Black() { return Color(SK_ColorBLACK); }
Color Color::White() { return Color(SK_ColorWHITE); }
Color Color::Red() { return Color(SK_ColorRED); }
Color Color::Green() { return Color(SK_ColorGREEN); }
Color Color::Blue() { return Color(SK_ColorBLUE); }
Color Color::Yellow() { return Color(SK_ColorYELLOW); }
Color Color::Cyan() { return Color(SK_ColorCYAN); }
Color Color::Magenta() { return Color(SK_ColorMAGENTA); }
Color Color::Gray() { return Color(SK_ColorGRAY); }
Color Color::LightGray() { return Color(SK_ColorLTGRAY); }
Color Color::DarkGray() { return Color(SK_ColorDKGRAY); }

Color Color::Random() {
  static std::mt19937_64 rng{std::random_device{}()};
  static std::uniform_int_distribution<uint8_t> dist(0, 255);

  uint8_t rr = dist(rng);
  uint8_t gg = dist(rng);
  uint8_t bb = dist(rng);
  return Color(rr, gg, bb, 255);
}
