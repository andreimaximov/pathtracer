#include "color.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

/**
 * * operator
 */

Color Color::operator*(float f) const {
  return Color(r * f, g * f, b * f);
}

Color Color::operator*(const Color& other) const {
  return Color(r * other.r, g * other.g, b * other.b);
}

Color& Color::operator*=(float f) {
  r *= f;
  g *= f;
  b *= f;
  return *this;
}

Color& Color::operator*=(const Color& other) {
  r *= other.r;
  g *= other.g;
  b *= other.b;
  return *this;
}

/**
 * + operator
 */

Color Color::operator+(float f) const {
  return Color(r + f, g + f, b + f);
}

Color Color::operator+(const Color& other) const {
  return Color(r + other.r, g + other.g, b + other.b);
}

Color& Color::operator+=(float f) {
  r += f;
  g += f;
  b += f;
  return *this;
}

Color& Color::operator+=(const Color& other) {
  r += other.r;
  g += other.g;
  b += other.b;
  return *this;
}

/**
 * Other
 */

Color Color::gamma(float gamma) const {
  assert(gamma > 0);
  return Color(std::pow(r, 1.0 / gamma), std::pow(g, 1.0 / gamma),
               std::pow(b, 1.0 / gamma));
}

Color Color::clamp() const {
  return Color(clamp(r), clamp(g), clamp(b));
}

bool Color::isBlack() const {
  return std::abs(r) < EPSILON && std::abs(g) < EPSILON &&
         std::abs(b) < EPSILON;
}

bool Color::isTransparent() const {
  return std::abs(a) < EPSILON;
}

float Color::max() const {
  return std::max(r, std::max(g, b));
}

float Color::clamp(float x) {
  return std::max(0.0f, std::min(1.0f, x));
}

const Color Color::BLACK = Color(0, 0, 0);
