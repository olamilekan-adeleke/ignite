#include "color.hpp"

#include <include/core/SkColor.h>

#include <random>
#include <sstream>

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

Color Color::withAlpha(uint8_t alpha) const { return Color(r(), g(), b(), alpha); }

// Basic colors
Color Color::Black() { return Color(0xFF000000); }
Color Color::White() { return Color(0xFFFFFFFF); }
Color Color::Clear() { return Color(0x00000000); }

// Primary colors
Color Color::Red() { return Color(0xFFFF0000); }
Color Color::Green() { return Color(0xFF00FF00); }
Color Color::Blue() { return Color(0xFF0000FF); }

// Secondary colors
Color Color::Yellow() { return Color(0xFFFFFF00); }
Color Color::Cyan() { return Color(0xFF00FFFF); }
Color Color::Magenta() { return Color(0xFFFF00FF); }

// Grayscale
Color Color::Gray() { return Color(0xFF808080); }
Color Color::LightGray() { return Color(0xFFD3D3D3); }
Color Color::DarkGray() { return Color(0xFFA9A9A9); }

// Additional common colors
Color Color::Orange() { return Color(0xFFFFA500); }
Color Color::Purple() { return Color(0xFF800080); }
Color Color::Pink() { return Color(0xFFFFC0CB); }
Color Color::Brown() { return Color(0xFFA52A2A); }
Color Color::Lime() { return Color(0xFF00FF00); }
Color Color::Teal() { return Color(0xFF008080); }
Color Color::Navy() { return Color(0xFF000080); }
Color Color::Maroon() { return Color(0xFF800000); }
Color Color::Olive() { return Color(0xFF808000); }
Color Color::Indigo() { return Color(0xFF4B0082); }
Color Color::Violet() { return Color(0xFFEE82EE); }
Color Color::Gold() { return Color(0xFFFFD700); }
Color Color::Silver() { return Color(0xFFC0C0C0); }

// Modern UI colors
Color Color::SlateGray() { return Color(0xFF708090); }
Color Color::DimGray() { return Color(0xFF696969); }
Color Color::Crimson() { return Color(0xFFDC143C); }
Color Color::Coral() { return Color(0xFFFF7F50); }
Color Color::Tomato() { return Color(0xFFFF6347); }
Color Color::SkyBlue() { return Color(0xFF87CEEB); }
Color Color::SteelBlue() { return Color(0xFF4682B4); }
Color Color::Turquoise() { return Color(0xFF40E0D0); }
Color Color::Aquamarine() { return Color(0xFF7FFFD4); }
Color Color::Salmon() { return Color(0xFFFFA07A); }
Color Color::Khaki() { return Color(0xFFF0E68C); }
Color Color::Plum() { return Color(0xFFDDA0DD); }
Color Color::Orchid() { return Color(0xFFDA70D6); }

Color Color::Random() {
  static std::mt19937_64 rng{std::random_device{}()};
  static std::uniform_int_distribution<uint8_t> dist(0, 255);
  uint8_t rr = dist(rng);
  uint8_t gg = dist(rng);
  uint8_t bb = dist(rng);
  return Color(rr, gg, bb, 255);
}

std::string Color::toString() const {
  std::ostringstream os;
  os << "Color {";
  os << "r: " << (int)r() << ", ";
  os << "g: " << (int)g() << ", ";
  os << "b: " << (int)b() << ", ";
  os << "a: " << (int)a();
  os << "}";
  return os.str();
}
