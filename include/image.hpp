#include <cstddef>
#include <vector>
#include "color.hpp"

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

class Image {
 private:
  size_t width;

  size_t height;

  float gamma = 1.0;

  std::vector<Color> pixels;

 public:
  /**
   * Creates a black image.
   */
  Image(size_t width, size_t height);

  /**
   * Creates an image from an array of RGBA pixels.
   */
  Image(size_t width, size_t height, unsigned char* pixels);

  /**
   * Gets the (x, y) pixel in the image.
   */
  Color get_pixel(size_t x, size_t y) const;

  /**
   * Gets the (i % width, i / width) pixel in the image.
   */
  Color get_pixel(size_t i) const;

  /**
   * Gets the (u * width, v * height) pixel in the image using UV coordinates.
   */
  Color get_pixel_uv(float u, float v) const;

  /**
   * Sets the (x, y) pixel in the image.
   */
  void set_pixel(size_t x, size_t y, const Color& pixel);

  /**
   * Sets the (i % width, i / width) pixel in the image.
   */
  void set_pixel(size_t i, const Color& pixel);

  /**
   * Returns the width of the image.
   */
  size_t get_width() const;

  /**
   * Returns the height of the image.
   */
  size_t get_height() const;

  /**
   * Gets the current gamma correction factor.
   */
  float get_gamma() const;

  /**
   * Sets the gamma correction factor when calling data() on the image.
   */
  void set_gamma(float gamma);

  /**
   * Returns the number of pixels in the image.
   */
  size_t size() const;

  /**
   * Returns a copy of the underlying 8-bit RGBA data of the image.
   */
  std::vector<unsigned char> data() const;
};

#endif  // IMAGE_HPP_
