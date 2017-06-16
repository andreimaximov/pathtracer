#ifndef COLOR_HPP_
#define COLOR_HPP_

struct Color {
  float r = 0;

  float g = 0;

  float b = 0;

  float a = 0;

  Color() = default;

  Color(float r, float g, float b, float a = 1.0);

  Color operator*(float f) const;

  Color operator*(const Color& other) const;

  Color& operator*=(float f);

  Color& operator*=(const Color& other);

  Color operator+(float f) const;

  Color operator+(const Color& other) const;

  Color& operator+=(float f);

  Color& operator+=(const Color& other);

  /**
   * Applies a gamma correction the the RGB channels of this color.
   */
  Color gamma(float gamma) const;

  /**
   * Clamps the channels of this color into the [0, 1] range.
   */
  Color clamp() const;

  /**
   * Indicates if the RGB channels are nearly (0, 0, 0).
   */
  bool isBlack() const;

  /**
   * Indicates if the color is transparent.
   */
  bool isTransparent() const;

  /**
   * Returns the values of the highest channel.
   */
  float max() const;

  /**
   * Clamps an RGBA channel into the [0, 1] range.
   */
  static float clamp(float f);

  /**
   * RGB channel epsilon.
   */
  static constexpr float EPSILON = 0.05;

  static const Color BLACK;
};

#endif  // COLOR_HPP_
