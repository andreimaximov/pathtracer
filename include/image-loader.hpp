#include <string>
#include "image.hpp"

#ifndef IMAGE_LOADER_HPP_
#define IMAGE_LOADER_HPP_

class ImageLoader {
 public:
  Image load(const std::string& filepath) const;
};

#endif  // IMAGE_LOADER_HPP_
