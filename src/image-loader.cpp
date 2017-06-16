#define STB_IMAGE_IMPLEMENTATION

#include "image-loader.hpp"
#include <stdexcept>
#include "stb_image.h"

Image ImageLoader::load(const std::string& filepath) const {
  int width, height, n;
  unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &n, 4);

  if (data == nullptr) {
    throw std::runtime_error("Error loading " + filepath + "!");
  }

  Image img(width, height, data);
  stbi_image_free(data);
  return img;
}
