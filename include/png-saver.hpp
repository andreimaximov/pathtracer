#include "image-saver.hpp"

#ifndef PNG_SAVER_HPP_
#define PNG_SAVER_HPP_

class PNGSaver : public ImageSaver {
 public:
  void save(const std::string& filepath, const Image& image) const;
};

#endif  // PNG_SAVER_HPP_
