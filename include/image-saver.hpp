#include <string>
#include "image.hpp"

#ifndef IMAGE_SAVER_HPP_
#define IMAGE_SAVER_HPP_

class ImageSaver {
 public:
  virtual void save(const std::string& filepath, const Image& image) const = 0;
};

#endif  // IMAGE_SAVER_HPP_
