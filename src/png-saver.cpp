#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "png-saver.hpp"
#include <stdexcept>
#include <string>
#include "stb_image_write.h"

void PNGSaver::save(const std::string& filepath, const Image& image) const {
  auto buffer = image.data();
  int status =
      stbi_write_png(filepath.c_str(), image.get_width(), image.get_height(), 4,
                     buffer.data(), image.get_width() * 4);
  if (status == 0) {
    throw std::runtime_error("Error saving image!");
  }
}
