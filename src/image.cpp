#include "image.hpp"

Image::Image(size_t width, size_t height)
    : width(width), height(height), pixels(width * height) {}

Image::Image(size_t width, size_t height, unsigned char* pixels)
    : width(width), height(height), pixels(width * height) {
  for (size_t i = 0; i < size(); i++) {
    set_pixel(i, Color(pixels[4 * i] / 255.0, pixels[4 * i + 1] / 255.0,
                       pixels[4 * i + 2] / 255.0, pixels[4 * i + 3] / 255.0));
  }
}

Color Image::get_pixel(size_t x, size_t y) const {
  return get_pixel((y * width) + x);
}

Color Image::get_pixel(size_t i) const {
  return pixels[i];
}

Color Image::get_pixel_uv(float u, float v) const {
  size_t x = u * width;
  size_t y = (1 - v) * height;

  // Wrap UV coordinates for repeated textures.
  return get_pixel((x % width + width) % width, (y % height + height) % height);
}

void Image::set_pixel(size_t x, size_t y, const Color& pixel) {
  set_pixel(y * width + x, pixel);
}

void Image::set_pixel(size_t i, const Color& pixel) {
  pixels[i] = pixel;
}

size_t Image::get_width() const {
  return width;
}

size_t Image::get_height() const {
  return height;
}

float Image::get_gamma() const {
  return gamma;
}

void Image::set_gamma(float gamma) {
  this->gamma = gamma;
}

size_t Image::size() const {
  return width * height;
}

std::vector<unsigned char> Image::data() const {
  std::vector<unsigned char> data(size() * 4);

  for (size_t i = 0; i < size(); i++) {
    Color p = get_pixel(i).clamp().gamma(gamma);
    data[i * 4] = 255 * p.r;
    data[i * 4 + 1] = 255 * p.g;
    data[i * 4 + 2] = 255 * p.b;
    data[i * 4 + 3] = 255 * p.a;
  }

  return data;
}
